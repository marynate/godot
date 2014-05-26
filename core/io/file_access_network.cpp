/*************************************************************************/
/*  file_access_network.cpp                                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "file_access_network.h"
#include "marshalls.h"
#include "globals.h"
#include "os/os.h"
#include "os/dir_access.h"
#include "io/ip.h"



#define DEBUG_PRINT(m_p) print_line(m_p)
//#define DEBUG_TIME(m_what) printf("MS: %s - %lli\n",m_what,OS::get_singleton()->get_ticks_usec());
//#define DEBUG_PRINT(m_p)
#define DEBUG_TIME(m_what)


void FileAccessNetworkClient::lock_mutex() {

	mutex->lock();
	lockcount++;
}

void FileAccessNetworkClient::unlock_mutex() {

	lockcount--;
	mutex->unlock();

}

void FileAccessNetworkClient::put_32(int p_32) {

	uint8_t buf[4];
	encode_uint32(p_32,buf);
	client->put_data(buf,4);
	DEBUG_PRINT("put32: "+itos(p_32));
}

void FileAccessNetworkClient::put_64(int64_t p_64) {

	uint8_t buf[8];
	encode_uint64(p_64,buf);
	client->put_data(buf,8);
	DEBUG_PRINT("put64: "+itos(p_64));

}

int FileAccessNetworkClient::get_32() {

	uint8_t buf[4];
	client->get_data(buf,4);
	return decode_uint32(buf);

}

int64_t FileAccessNetworkClient::get_64() {

	uint8_t buf[8];
	client->get_data(buf,8);
	return decode_uint64(buf);

}

void FileAccessNetworkClient::_thread_func() {

	client->set_nodelay(true);
	while(!quit) {

		DEBUG_PRINT("SEM WAIT - "+itos(sem->get()));
		Error err = sem->wait();
		DEBUG_TIME("sem_unlock");
		//DEBUG_PRINT("semwait returned "+itos(werr));
		DEBUG_PRINT("MUTEX LOCK "+itos(lockcount));
		DEBUG_PRINT("POPO");
		DEBUG_PRINT("PEPE");
		lock_mutex();
		DEBUG_PRINT("MUTEX PASS");

		blockrequest_mutex->lock();
		while(block_requests.size()) {
			put_32(block_requests.front()->get().id);
			put_32(FileAccessNetwork::COMMAND_READ_BLOCK);
			put_64(block_requests.front()->get().offset);
			put_32(block_requests.front()->get().size);
			block_requests.pop_front();
		}
		blockrequest_mutex->unlock();

		DEBUG_PRINT("THREAD ITER");

		DEBUG_TIME("sem_read");
		int id = get_32();

		int response = get_32();
		DEBUG_PRINT("GET RESPONSE: "+itos(response));

		FileAccessNetwork *fa=NULL;

		if (response!=FileAccessNetwork::RESPONSE_DATA) {
			ERR_FAIL_COND(!accesses.has(id));
		}

		if (accesses.has(id))
			fa=accesses[id];


		switch(response) {

			case FileAccessNetwork::RESPONSE_OPEN: {


				DEBUG_TIME("sem_open");
				int status = get_32();
				if (status!=OK) {
					fa->_respond(0,Error(status));
				} else {
					uint64_t len = get_64();
					fa->_respond(len,Error(status));
				}

				fa->sem->post();


			} break;
			case FileAccessNetwork::RESPONSE_DATA: {

				int64_t offset = get_64();
				uint32_t len = get_32();

				Vector<uint8_t> block;
				block.resize(len);
				client->get_data(block.ptr(),len);

				if (fa) //may have been queued
					fa->_set_block(offset,block);

			} break;
			case FileAccessNetwork::RESPONSE_FILE_EXISTS: {


				int status = get_32();
				fa->exists_modtime=status!=0;
				fa->sem->post();



			} break;
			case FileAccessNetwork::RESPONSE_GET_MODTIME: {


				uint64_t status = get_64();
				fa->exists_modtime=status;
				fa->sem->post();

			} break;

		}


		unlock_mutex();
	}

}

void FileAccessNetworkClient::_thread_func(void *s) {

	FileAccessNetworkClient *self =(FileAccessNetworkClient*)s;

	self->_thread_func();

}

Error FileAccessNetworkClient::connect(const String& p_host,int p_port,const String& p_password) {

	IP_Address ip;

	if (p_host.is_valid_ip_address()) {
		ip=p_host;
	} else {
		ip=IP::get_singleton()->resolve_hostname(p_host);
	}

	DEBUG_PRINT("IP: "+String(ip)+" port "+itos(p_port));
	Error err = client->connect(ip,p_port);
	ERR_FAIL_COND_V(err,err);
	while(client->get_status()==StreamPeerTCP::STATUS_CONNECTING) {
//DEBUG_PRINT("trying to connect....");
		OS::get_singleton()->delay_usec(1000);
	}

	if (client->get_status()!=StreamPeerTCP::STATUS_CONNECTED) {
		return ERR_CANT_CONNECT;
	}

	CharString cs = p_password.utf8();	
	put_32(cs.length());
	client->put_data((const uint8_t*)cs.ptr(),cs.length());

	int e = get_32();

	if (e!=OK) {
		return ERR_INVALID_PARAMETER;
	}

	thread = Thread::create(_thread_func,this);

	return OK;
}

FileAccessNetworkClient *FileAccessNetworkClient::singleton=NULL;


FileAccessNetworkClient::FileAccessNetworkClient() {

	thread=NULL;
	mutex = Mutex::create();
	blockrequest_mutex = Mutex::create();
	quit=false;
	singleton=this;
	last_id=0;
	client = Ref<StreamPeerTCP>( StreamPeerTCP::create_ref() );
	sem=Semaphore::create();
	lockcount=0;
}

FileAccessNetworkClient::~FileAccessNetworkClient() {

	if (thread) {
		quit=true;
		sem->post();
		Thread::wait_to_finish(thread);
	}

	memdelete(blockrequest_mutex);
	memdelete(mutex);
	memdelete(sem);


}

void FileAccessNetwork::_set_block(size_t p_offset,const Vector<uint8_t>& p_block) {


	int page = p_offset/page_size;
	ERR_FAIL_INDEX(page,pages.size());
	if (page<pages.size()-1) {
		ERR_FAIL_COND(p_block.size()!=page_size);
	} else {
		ERR_FAIL_COND( (p_block.size() != (total_size%page_size)));
	}

	buffer_mutex->lock();
	pages[page].buffer=p_block;
	pages[page].queued=false;
	buffer_mutex->unlock();

	if (waiting_on_page==page) {
		waiting_on_page=-1;
		page_sem->post();
	}
}


void FileAccessNetwork::_respond(size_t p_len,Error p_status) {

	DEBUG_PRINT("GOT RESPONSE - len: "+itos(p_len)+" status: "+itos(p_status));
	response=p_status;
	if (response!=OK)
		return;
	opened=true;
	total_size=p_len;
	int pc = ((total_size-1)/page_size)+1;
	pages.resize(pc);




}

Error FileAccessNetwork::_open(const String& p_path, int p_mode_flags) {

	ERR_FAIL_COND_V(p_mode_flags!=READ,ERR_UNAVAILABLE);
	if (opened)
		close();
	FileAccessNetworkClient *nc = FileAccessNetworkClient::singleton;
	DEBUG_PRINT("open: "+p_path);

	DEBUG_TIME("open_begin");

	nc->lock_mutex();
	nc->put_32(id);
	nc->accesses[id]=this;
	nc->put_32(COMMAND_OPEN_FILE);
	CharString cs =p_path.utf8();
	nc->put_32(cs.length());
	nc->client->put_data((const uint8_t*)cs.ptr(),cs.length());
	pos=0;
	eof_flag=false;
	last_page=-1;
	last_page_buff=NULL;

//	buffers.clear();
	nc->unlock_mutex();
	DEBUG_PRINT("OPEN POST");
	DEBUG_TIME("open_post");
	nc->sem->post(); //awaiting answer
	DEBUG_PRINT("WAIT...");
	sem->wait();
	DEBUG_TIME("open_end");
	DEBUG_PRINT("WAIT ENDED...");

	return response;
}

void FileAccessNetwork::close(){

	if (!opened)
		return;

	FileAccessNetworkClient *nc = FileAccessNetworkClient::singleton;

	DEBUG_PRINT("CLOSE");
	nc->lock_mutex();
	nc->put_32(id);
	nc->put_32(COMMAND_CLOSE);
	pages.clear();
	opened=false;
	nc->unlock_mutex();


}
bool FileAccessNetwork::is_open() const{

	return opened;
}

void FileAccessNetwork::seek(size_t p_position){

	ERR_FAIL_COND(!opened);
	eof_flag=p_position>total_size;

	if (p_position>=total_size) {
		p_position=total_size;
	}

	pos=p_position;
}

void FileAccessNetwork::seek_end(int64_t p_position){

	seek(total_size+p_position);

}
size_t FileAccessNetwork::get_pos() const{

	ERR_FAIL_COND_V(!opened,0);
	return pos;
}
size_t FileAccessNetwork::get_len() const{

	ERR_FAIL_COND_V(!opened,0);
	return total_size;
}

bool FileAccessNetwork::eof_reached() const{

	ERR_FAIL_COND_V(!opened,false);
	return eof_flag;
}

uint8_t FileAccessNetwork::get_8() const{

	uint8_t v;
	get_buffer(&v,1);
	return v;

}


void FileAccessNetwork::_queue_page(int p_page) const {

	if (p_page>=pages.size())
		return;
	if (pages[p_page].buffer.empty() && !pages[p_page].queued) {


		FileAccessNetworkClient *nc = FileAccessNetworkClient::singleton;

		nc->blockrequest_mutex->lock();
		FileAccessNetworkClient::BlockRequest br;
		br.id=id;
		br.offset=size_t(p_page)*page_size;
		br.size=page_size;
		nc->block_requests.push_back(br);
		pages[p_page].queued=true;
		nc->blockrequest_mutex->unlock();
		DEBUG_PRINT("QUEUE PAGE POST");
		nc->sem->post();
		DEBUG_PRINT("queued "+itos(p_page));
	}

}

int FileAccessNetwork::get_buffer(uint8_t *p_dst, int p_length) const{

	//bool eof=false;
	if (pos+p_length>total_size) {
		eof_flag=true;
	}
	if (pos+p_length>=total_size) {
		p_length=total_size-pos;
	}

//	FileAccessNetworkClient *nc = FileAccessNetworkClient::singleton;

	uint8_t *buff=last_page_buff;

	for(int i=0;i<p_length;i++) {

		int page=pos/page_size;

		if (page!=last_page) {
			buffer_mutex->lock();
			if (pages[page].buffer.empty()) {
				//fuck

				waiting_on_page=page;
				for(int j=0;j<read_ahead;j++) {

					_queue_page(page+j);
				}
				buffer_mutex->unlock();
				DEBUG_PRINT("wait");
				page_sem->wait();
				DEBUG_PRINT("done");
			} else {

				for(int j=0;j<read_ahead;j++) {

					_queue_page(page+j);
				}
				buff=pages[page].buffer.ptr();
				//queue pages
				buffer_mutex->unlock();
			}

			buff=pages[page].buffer.ptr();
			last_page_buff=buff;
			last_page=page;
		}

		p_dst[i]=buff[pos-uint64_t(page)*page_size];
		pos++;
	}

	return p_length;
}

Error FileAccessNetwork::get_error() const{

	return pos==total_size?ERR_FILE_EOF:OK;
}

void FileAccessNetwork::store_8(uint8_t p_dest) {

	ERR_FAIL();
}

bool FileAccessNetwork::file_exists(const String& p_path){

	FileAccessNetworkClient *nc = FileAccessNetworkClient::singleton;
	nc->lock_mutex();
	nc->put_32(id);
	nc->put_32(COMMAND_FILE_EXISTS);
	CharString cs=p_path.utf8();
	nc->put_32(cs.length());
	nc->client->put_data((const uint8_t*)cs.ptr(),cs.length());
	nc->unlock_mutex();
	DEBUG_PRINT("FILE EXISTS POST");
	nc->sem->post();
	sem->wait();

	return exists_modtime!=0;

}

uint64_t FileAccessNetwork::_get_modified_time(const String& p_file){

	FileAccessNetworkClient *nc = FileAccessNetworkClient::singleton;
	nc->lock_mutex();
	nc->put_32(id);
	nc->put_32(COMMAND_GET_MODTIME);
	CharString cs=p_file.utf8();
	nc->put_32(cs.length());
	nc->client->put_data((const uint8_t*)cs.ptr(),cs.length());
	nc->unlock_mutex();
	DEBUG_PRINT("MODTIME POST");
	nc->sem->post();
	sem->wait();

	return exists_modtime;

}

FileAccessNetwork::FileAccessNetwork() {

	eof_flag=false;
	opened=false;
	pos=0;
	sem=Semaphore::create();
	page_sem=Semaphore::create();
	buffer_mutex=Mutex::create();
	FileAccessNetworkClient *nc = FileAccessNetworkClient::singleton;
	nc->lock_mutex();
	id=nc->last_id++;
	nc->accesses[id]=this;
	nc->unlock_mutex();
	page_size = GLOBAL_DEF("remote_fs/page_size",65536);
	read_ahead = GLOBAL_DEF("remote_fs/page_read_ahead",4);
	max_pages = GLOBAL_DEF("remote_fs/max_pages",20);
	last_activity_val=0;
	waiting_on_page=-1;
	last_page=-1;


}

FileAccessNetwork::~FileAccessNetwork() {

	close();
	memdelete(sem);
	memdelete(page_sem);
	memdelete(buffer_mutex);

	FileAccessNetworkClient *nc = FileAccessNetworkClient::singleton;
	nc->lock_mutex();
	id=nc->last_id++;
	nc->accesses.erase(id);
	nc->unlock_mutex();

}

String FileAccessCachedNetwork::cache_dir="";
uint64_t FileAccessCachedNetwork::time_margin = 5; // seconds;
Map<String, uint64_t> FileAccessCachedNetwork::remote_fscache;
bool FileAccessCachedNetwork::use_remote_fscache=false;

Error FileAccessCachedNetwork::_ensure_dir(const String& base_dir, const String& p_rel_path) {

	if (p_rel_path == "")
		return OK;

	DirAccess *da = DirAccess::create_for_path(base_dir);
	String dir = DirAccess::normalize_path(p_rel_path);

	if (dir.length()<1)
		return OK;

	// expected format "foo/bar/"
	if (dir[dir.length()-1]!='/')
		dir = dir + "/";
	if (dir[0]=='/')
		dir = dir.right(1);

	da->change_dir(base_dir);
	int pos = 0;
	while (pos<dir.length()) {
		int n = dir.find("/", pos);
		if (n==-1) break;
		String current = dir.substr(pos, n-pos);
		Error err = da->make_dir(current);
		if (err!=OK && err!= ERR_ALREADY_EXISTS) {
			ERR_FAIL_V(err);
		}
		da->change_dir(current);
		pos = n+1;
	}

	memdelete(da);
	return OK;
}

String FileAccessCachedNetwork::_get_cached_path(const String& p_path) {
	String cached_path = p_path;
	print_line("OS::get_singleton()->get_data_dir() is: " + OS::get_singleton()->get_data_dir());
	if (cache_dir=="") {
		String appname = Globals::get_singleton()->get("application/name");
		if (appname == "") appname = "noname";

		String data_path = OS::get_singleton()->get_data_dir();

		if (data_path=="") {
			data_path = ".";
		} else {
			String base = data_path.get_base_dir();
			String data_dir = data_path.replace(base,"").replace("\\","/");
			_ensure_dir(base, data_dir); //  make sure data_dir is created for OS might haven not been initialized at this point
		}
		cache_dir = data_path + "/rfscache/" + appname + "/";
		_ensure_dir(data_path, "/rfscache/" + appname + "/");
	}

	if (cached_path.begins_with("res://")) {
		_ensure_dir(cache_dir, cached_path.get_base_dir().replace("res://",""));
		return cached_path.replace_first("res://", cache_dir);
	}

	print_line("_get_cached_path: " + p_path + " => " + cached_path);
	return cached_path;
}

Error FileAccessCachedNetwork::_open(const String& p_path, int p_mode_flags) {

	print_line("[FileAccessCachedNetwork] opening " + p_path);
	String cached_path = _get_cached_path(p_path);
	bool is_cached = false;

	Error err_cache;
	fa_cache = FileAccess::open(cached_path, p_mode_flags, &err_cache);
	cache_opened = err_cache==OK && fa_cache->file_exists(cached_path);

	bool need_update = false;
	if (cache_opened) {
		uint64_t local_time = FileAccess::get_modified_time(cached_path);
		FileAccessNetwork *fan_tmp = memnew(FileAccessNetwork);
		uint64_t remote_time = use_remote_fscache? remote_fscache[p_path] : fan_tmp->_get_modified_time(p_path);
		need_update = local_time < remote_time + time_margin;
		print_line("  => local: " + itos(local_time) + " - remote: "  + itos(remote_time) + " => " + (need_update?"UPDATE":"CACHED"));
		fan_tmp->close();
		memdelete(fan_tmp);
	}

	Error err;
	if ( !cache_opened || (cache_opened && need_update) ) {

		print_line("  => Try updating from remote");
		Error err_remote = fa_remote->_open(p_path, p_mode_flags);

		if (err_remote!=OK && !cache_opened) {
			print_line("  => Can't get file from remote and cache => Error: "  + (err_remote==ERR_FILE_NOT_FOUND?"ERR_FILE_NOT_FOUND":itos(err_remote)));
			return err_remote;
		}

		if (err_remote!=OK && err_remote!=ERR_FILE_NOT_FOUND && cache_opened) {
			print_line("  => Can't access remote copy (err:" + itos(err_remote) + "), use cached version instead! ");
			fa_remote->close();
			return OK;
		}

		if (/*err_remote!=OK*/err_remote == ERR_FILE_NOT_FOUND && cache_opened) {
			print_line("  => File not found in remote, deleting local cache");
			fa_cache->close();
			cache_opened=false;
			DirAccess *da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
			da->remove(cached_path);
			memdelete(da);
			return ERR_FILE_NOT_FOUND;
		}

		Vector<uint8_t> buf;
		buf.resize(fa_remote->get_len());
		fa_remote->get_buffer(buf.ptr(),fa_remote->get_len());

		if (cache_opened)
			fa_cache->close();


		FileAccess *write = FileAccess::open(cached_path, FileAccess::WRITE, &err);
		if (err!=OK) { // can't copy, use remote
			cache_opened = false;
			return OK;
		} else {
			write->store_buffer(buf.ptr(),buf.size());
			write->close();
			memdelete(write);

			fa_cache = FileAccess::open(cached_path, p_mode_flags, &err); // reopen with requested flags
			cache_opened = (err==OK);
		}
	}

	if (cache_opened) {
		fa_remote->close();
		return OK;
	}

	return err;
}

void FileAccessCachedNetwork::close(){

	if (cache_opened)
		fa_cache->close();
	else
		fa_remote->close();
}

bool FileAccessCachedNetwork::is_open() const{

	if (cache_opened)
		return true;
	return fa_remote->is_open();
}

void FileAccessCachedNetwork::seek(size_t p_position){

	if (cache_opened)
		return fa_cache->seek(p_position);
	return fa_remote->seek(p_position);
}

void FileAccessCachedNetwork::seek_end(int64_t p_position){

	if (cache_opened)
		return fa_cache->seek_end(p_position);
	return fa_remote->seek_end(p_position);
}

size_t FileAccessCachedNetwork::get_pos() const{

	if (cache_opened)
		return fa_cache->get_pos();
	return fa_remote->get_pos();
}

size_t FileAccessCachedNetwork::get_len() const{

	if (cache_opened)
		return fa_cache->get_len();
	return fa_remote->get_len();
}

bool FileAccessCachedNetwork::eof_reached() const{

	if (cache_opened)
		return fa_cache->eof_reached();
	return fa_remote->eof_reached();
}

uint8_t FileAccessCachedNetwork::get_8() const{

	if (cache_opened)
		return fa_cache->get_8();
	return fa_remote->get_8();
}

int FileAccessCachedNetwork::get_buffer(uint8_t *p_dst, int p_length) const{

	if (cache_opened)
		return fa_cache->get_buffer(p_dst, p_length);
	return fa_remote->get_buffer(p_dst, p_length);
}

Error FileAccessCachedNetwork::get_error() const{

	if (cache_opened)
		return fa_cache->get_error();
	return fa_remote->get_error();
}

void FileAccessCachedNetwork::store_8(uint8_t p_dest) {

	if (cache_opened)
		return fa_cache->store_8(p_dest);
	return fa_remote->store_8(p_dest);
}

bool FileAccessCachedNetwork::file_exists(const String& p_path){

	if (cache_opened)
		return fa_cache->file_exists(_get_cached_path(p_path));
	return fa_remote->file_exists(p_path);

}

uint64_t FileAccessCachedNetwork::_get_modified_time(const String& p_file){

	if (cache_opened)
		return FileAccess::get_modified_time(_get_cached_path(p_file));
	return fa_remote->_get_modified_time(p_file);
}

void FileAccessCachedNetwork::setup() {

	use_remote_fscache = false;

	// Cache files' modified time from remote .fscache
	FileAccess::make_default<FileAccessNetwork>(FileAccess::ACCESS_RESOURCES);

	// It's necessay to trigger .fscache update in remote editor manually to make changes reflected here
	// (by press "reload" button in FileSystem tab)
	// todo: update .fscache each time scene get saved
	// todo: cache .cfg for offline playback
	FileAccess *f = FileAccess::open("res://.fscache", FileAccess::READ);
	if (f) {
		String last_dir = "";
		while(!f->eof_reached()) {
			String l = f->get_line().strip_edges();
			if (l==String())
				continue;

			if (l.begins_with("::")) {
				Vector<String> split = l.split("::");
				ERR_CONTINUE( split.size() != 3);
				last_dir = split[1];
			} else {
				Vector<String> split = l.split("::");
				ERR_CONTINUE( split.size() != 4);
				String file_path = (last_dir=="res://")? (last_dir+split[0]) : (last_dir+"/"+split[0]);
				uint64_t modification_time=split[2].to_int64();
				remote_fscache.insert(file_path, modification_time);
				print_line(" adding: " + file_path + ": " + itos(modification_time));
			}
		}
		f->close();
		memdelete(f);

		use_remote_fscache = remote_fscache.size()>0;
	}	

	FileAccess::make_default<FileAccessCachedNetwork>(FileAccess::ACCESS_RESOURCES);
}

FileAccessCachedNetwork::FileAccessCachedNetwork() {

	cache_opened = false;
	fa_remote = memnew(FileAccessNetwork);
}

FileAccessCachedNetwork::~FileAccessCachedNetwork() {
	if (fa_cache)
		memdelete(fa_cache);
	if (fa_remote)
		memdelete(fa_remote);
}

