#include "stereo_kit.h"

#include "d3d.h"
#include "render.h"

#include "win32.h"
#include "openxr.h"

#include <thread> // sleep_for

using namespace std;

sk_runtime_ sk_runtime = sk_runtime_flatscreen;
bool sk_focused = true;
bool sk_run     = true;

float  sk_timef = 0;
double sk_time  = 0;
double sk_time_start    = 0;
double sk_time_elapsed  = 0;
float  sk_time_elapsedf = 0;
long long sk_time_raw = 0;

bool sk_init(const char *app_name, sk_runtime_ runtime) {
	sk_runtime = runtime;

	d3d_init();
	
	bool result = true;
	switch (sk_runtime) {
	case sk_runtime_flatscreen:   result = win32_init (app_name); break;
	case sk_runtime_mixedreality: result = openxr_init(app_name); break;
	}
	if (!result)
		return false;

	render_initialize();
	return true;
}

void sk_shutdown() {
	render_shutdown();
	switch (sk_runtime) {
	case sk_runtime_flatscreen:   win32_shutdown (); break;
	case sk_runtime_mixedreality: openxr_shutdown(); break;
	}
	d3d_shutdown();
}

void sk_update_timer() {
	FILETIME time;
	GetSystemTimePreciseAsFileTime(&time);
	sk_time_raw = (LONGLONG)time.dwLowDateTime + ((LONGLONG)(time.dwHighDateTime) << 32LL);
	double time_curr = sk_time_raw / 10000000.0;

	if (sk_time_start == 0)
		sk_time_start = time_curr;
	double new_time = time_curr - sk_time_start;
	sk_time_elapsed  = new_time - sk_time;
	sk_time          = new_time;
	sk_time_elapsedf = (float)sk_time_elapsed;
	sk_timef         = (float)sk_time;
}

bool sk_step(void (*app_update)(void)) {
	switch (sk_runtime) {
	case sk_runtime_flatscreen:   win32_step_begin (); break;
	case sk_runtime_mixedreality: openxr_step_begin(); break;
	}
	
	sk_update_timer();
	app_update();

	d3d_render_begin();
	switch (sk_runtime) {
	case sk_runtime_flatscreen:   win32_step_end (); break;
	case sk_runtime_mixedreality: openxr_step_end(); break;
	}
	d3d_render_end();

	this_thread::sleep_for(chrono::milliseconds(sk_focused?1:250));

	return sk_run;
}