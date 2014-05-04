#include "object_type_db.h"
#include "core/globals.h"
#include "admob_ios.h"

void register_admob_types() {

	Globals::get_singleton()->add_singleton(Globals::Singleton("AdMob", memnew(AdMob_iOS)));

};

void unregister_admob_types() {

};
