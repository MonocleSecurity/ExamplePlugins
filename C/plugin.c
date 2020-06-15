
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

#include "libplugin.h"

MonocleGetRecordings monocle_get_recordings = NULL;
MonocleGetLatLong monocle_get_latlong = NULL;
MonocleGetSnapshot monocle_get_snapshot_ = NULL;
MonocleFree monocle_free = NULL;

static void write_file()
{
  FILE* file = fopen("c.txt", "w+");
  if (!file)
  {
    printf("Failed to open file: c.txt\n");
    fflush(stdout);
    return;
  }

  struct LATLONG* latlong = monocle_get_latlong();
  if (latlong)
  {
    fprintf(file, "Latitude:%f Longitude:%f\n", latlong->latitude_, latlong->longitude_);
    monocle_free(latlong);
    latlong = NULL;
  }

  struct RECORDING* recordings = NULL;
  size_t numrecordings = 0;
  monocle_get_recordings(&recordings, &numrecordings);
  for (size_t i = 0; i < numrecordings; ++i)
  {
    struct RECORDING* recording = &recordings[i];
    fprintf(file,
            "Recording: token:%" PRIu64 " name:%s location:%s retentiontime:%" PRIu64 " adaptivestreaming:%d relativelocation:%d orientation:%f",
            recording->token_,
            recording->name_,
            recording->location_,
            recording->retention_time_,
            recording->adaptive_streaming_,
            recording->relative_location_,
            recording->orientation_);
    if (recording->latlong_)
    {
      fprintf(file, " latitude:%f longitude:%f", recording->latlong_->latitude_, recording->latlong_->longitude_);

    }
    fprintf(file, "\n");

    for (size_t j = 0; j < recording->numtracks_; ++j)
    {
      struct TRACK* track = &recording->tracks_[j];
      fprintf(file, "  Track: id:%u track_type:%d description:%s state:%d error:%s mediauri:%s parameters:", track->id_, track->track_type_, track->description_, track->state_, track->error_, track->mediauri_);
      for (size_t k = 0; k < track->num_parameters_; ++k)
      {
        fprintf(file, " %s", track->parameters_[k]);

      }
      fprintf(file, " activeparameters:");
      for (size_t k = 0; k < track->num_active_parameters_; ++k)
      {
        fprintf(file, " %s", track->active_parameters_[k]);

      }
      fprintf(file, "\n");
    }
  }
  monocle_free(recordings);
  fclose(file);
}

MODULE_API int monocle_plugin_init(struct PLUGINFUNCTIONS* pluginfunctions, const char* parameters)
{
  monocle_get_recordings = pluginfunctions->monocle_get_recordings_;
  monocle_get_latlong = pluginfunctions->monocle_get_latlong_;
  monocle_get_snapshot_ = pluginfunctions->monocle_get_snapshot_;
  monocle_free = pluginfunctions->monocle_free_;
  write_file();
  return 0;
}

MODULE_API void monocle_plugin_destroy()
{

}

MODULE_API void monocle_recording_added(struct RECORDING* recording)
{
  write_file();

}

MODULE_API void monocle_recording_changed(struct RECORDING* recording)
{
  write_file();

}

MODULE_API void monocle_recording_removed(uint64_t token)
{
  write_file();

}

MODULE_API void monocle_latlong_changed(struct LATLONG* latlong)
{
  write_file();

}

MODULE_API void monocle_object_entered(uint64_t recordingtoken, uint32_t trackid, uint64_t time, uint64_t id, enum OBJECT_CLASS classid, float confidence, float x, float y, float width, float height)
{
  char* data = NULL;
  size_t size = 0;
  if (monocle_get_snapshot_(recordingtoken, trackid, time, x, y, width, height, &data, &size))
  {
    printf("Failed to get snapshot\n");
    fflush(stdout);
    return;
  }

  char buf[256];
  sprintf(buf, "%" PRIu64 "_%" PRIu64 ".png", time, id);
  FILE* file = fopen(buf, "wb+");
  if (!file)
  {
    printf("Failed to open file: %s\n", buf);
    fflush(stdout);
    return;
  }
  else
  {
    if (fwrite(data, 1, size, file) != size)
    {
      printf("Failed to write file: %s\n", buf);
      fflush(stdout);
    }
    fclose(file);
  }

  monocle_free(data);
}

MODULE_API void monocle_object_moved(uint64_t recordingtoken, uint32_t trackid, uint64_t time, uint64_t id, enum OBJECT_CLASS classid, float confidence, float x, float y, float width, float height)
{

}

MODULE_API void monocle_object_exited(uint64_t recordingtoken, uint32_t trackid, uint64_t time, uint64_t id, enum OBJECT_CLASS classid)
{

}
