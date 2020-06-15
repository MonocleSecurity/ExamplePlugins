// libplugin.hpp
//

#ifndef IDEWLZX5ZXDCZSDSDSSZZZZZD0WNOKQD6C
#define IDEWLZX5ZXDCZSDSDSSZZZZZD0WNOKQD6C

#ifdef _WIN32
  #ifndef LIBPLUGIN_EXPORT
    #define MODULE_API __declspec(dllexport)
  #endif
#else
  #define MODULE_API
#endif

///// Includes /////

#include <stdint.h>

///// Enumerations /////

enum OBJECT_CLASS
{
  OBJECT_CLASS_HUMAN = 0,
  OBJECT_CLASS_BICYCLE = 1,
  OBJECT_CLASS_CAR = 2,
  OBJECT_CLASS_MOTORBIKE = 3,
  OBJECT_CLASS_AEROPLANE = 4,
  OBJECT_CLASS_BUS = 5,
  OBJECT_CLASS_TRAIN = 6,
  OBJECT_CLASS_TRUCK = 7,
  OBJECT_CLASS_BOAT = 8,
  OBJECT_CLASS_CAT = 15,
  OBJECT_CLASS_DOG = 16,
  OBJECT_CLASS_HORSE = 17,
  OBJECT_CLASS_BACKPACK = 24,
  OBJECT_CLASS_UMBRELLA = 25,
  OBJECT_CLASS_HANDBAG = 26,
  OBJECT_CLASS_SUITCASE = 29
};

enum TRACK_TYPE
{
  TRACK_TYPE_VIDEO = 0,
  TRACK_TYPE_AUDIO = 1,
  TRACK_TYPE_METADATA = 2,
  TRACK_TYPE_OBJECT_DETECTOR = 3
};

enum TRACK_STATE
{
  TRACK_STATE_IDLE = 0,
  TRACK_STATE_ACTIVE = 1,
  TRACK_STATE_ERROR = 2,
  TRACK_STATE_ACTIVE_NOT_RECORDING = 3,
};

///// Declarations /////

struct LATLONG;
struct PLUGINFUNCTIONS;
struct RECORDING;
struct TRACK;

typedef int(*MonoclePluginInit)(struct PLUGINFUNCTIONS*, const char*);
typedef void(*MonoclePluginDestroy)();
typedef void(*MonocleRecordingAdded)(struct RECORDING* recording);
typedef void(*MonocleRecordingChanged)(struct RECORDING* recording);
typedef void(*MonocleRecordingRemoved)(uint64_t token);
typedef void(*MonocleLatLongChanged)(struct LATLONG* latlong);
typedef void(*MonocleObjectEntered)(uint64_t recordingtoken, uint32_t trackid, uint64_t time, uint64_t id, enum OBJECT_CLASS classid, float confidence, float x, float y, float width, float height);
typedef void(*MonocleObjectMoved)(uint64_t recordingtoken, uint32_t trackid, uint64_t time, uint64_t id, enum OBJECT_CLASS classid, float confidence, float x, float y, float width, float height);

typedef void(*MonocleGetRecordings)(struct RECORDING** recordings, size_t* numrecordings); // User must free the recordings
typedef struct LATLONG*(*MonocleGetLatLong)();  // User must free the returned structure
typedef int (*MonocleGetSnapshot)(uint64_t recordingtoken, uint32_t trackid, uint64_t time, float x, float y, float width, float height, char** data, size_t* size); // Returns 0 on success
typedef void(*MonocleFree)(void* ptr);

///// Structures /////

#pragma pack(1)
struct LATLONG
{
  double latitude_;
  double longitude_;
};

struct PLUGINFUNCTIONS
{
  MonocleGetRecordings monocle_get_recordings_;
  MonocleGetLatLong monocle_get_latlong_;
  MonocleGetSnapshot monocle_get_snapshot_;
  MonocleFree monocle_free_;
};

struct TRACK
{
  uint32_t id_;
  enum TRACK_TYPE track_type_;
  char* description_;
  enum TRACK_STATE state_;
  char* error_;
  char* mediauri_;
  char** parameters_;
  size_t num_parameters_;
  char** active_parameters_;
  size_t num_active_parameters_;
};

struct RECORDING
{
  uint64_t token_;
  char* name_;
  char* location_;
  uint64_t retention_time_;
  int adaptive_streaming_;
  int relative_location_;
  double orientation_;
  struct LATLONG* latlong_;
  struct TRACK* tracks_;
  size_t numtracks_;
};
#pragma pack()

///// Prototypes /////

#ifdef __cplusplus
extern "C"
{
#endif
#ifndef LIBPLUGIN_EXPORT
  MODULE_API int monocle_plugin_init(struct PLUGINFUNCTIONS* pluginfunctions, const char* parameters); // User must define this, pluginfunctions contains functions the user may wish to call
  MODULE_API void monocle_plugin_destroy(); // User should define this, this is called when the plugin is unloaded
  MODULE_API void monocle_recording_added(struct RECORDING* recording); // User may define this. This is called when a recording is added
  MODULE_API void monocle_recording_changed(struct RECORDING* recording); // User may define this. This is called when a recording is changed
  MODULE_API void monocle_recording_removed(uint64_t token); // User may define this. This is called when a recording is removed
  MODULE_API void monocle_latlong_changed(struct LATLONG* latlong); // User may define this. This is called when the latitude or longitude of the device changes
  MODULE_API void monocle_object_entered(uint64_t recordingtoken, uint32_t trackid, uint64_t time, uint64_t id, enum OBJECT_CLASS classid, float confidence, float x, float y, float width, float height); // User may define this. This is called when an object is first identified
  MODULE_API void monocle_object_moved(uint64_t recordingtoken, uint32_t trackid, uint64_t time, uint64_t id, enum OBJECT_CLASS classid, float confidence, float x, float y, float width, float height); // User may define this. This is called when an object that has entered has moved
#endif
#ifdef __cplusplus
}
#endif

#endif
