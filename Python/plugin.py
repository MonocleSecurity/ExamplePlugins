import monocle;
import sys;

def write_file():
  file = open("python.txt", "w+")
  # LatLong
  latlong = monocle.get_latlong();
  try:
    latlong.latitude
    latlong.longitude
  except:
    pass
  else:
    file.write("latitude:" + str(latlong.latitude) + " longitude:" + str(latlong.longitude) + "\n")
  # Recordings
  for recording in monocle.get_recordings():
    file.write("Recording: token:" + str(recording.token) + " name:" + recording.name + " location:" + recording.location + " retention_time:" + str(recording.retention_time) + " adaptive_streaming:" + str(recording.adaptive_streaming) + " relative_location:" + str(recording.relative_location) + " orientation:" + str(recording.orientation))
    try:
      recording.latlong.latitude
      recording.latlong.longitude
    except:
      pass
    else:
      file.write(" latitude:" + str(recording.latlong.latitude) + " longitude:" + str(recording.latlong.longitude))
    file.write("\n")
    for track in recording.tracks:
      parameters = ""
      for parameter in track.parameters:
        parameters += parameter
      active_parameters = ""
      for active_parameter in track.active_parameters:
        active_parameters += active_parameter
      file.write("  Track: id:" + str(track.id) + " track_type:" + str(track.track_type) + " description:" + track.description + " state:" + str(track.state) + " error:" + track.error + " mediauri:" + track.mediauri + " parameters:" + parameters + " active_parameters:" + active_parameters + "\n")

def monocle_init(parameters):
  write_file()

def monocle_destroy():
  pass

def recording_added(recording):
  write_file()

def recording_changed(recording):
  write_file()

def recording_removed(token):
  write_file()

def latlong_changed(latlong):
  write_file()

def object_entered(recordingtoken, trackid, time, id, classid, confidence, x, y, width, height):
  pass
  
def object_exited(recordingtoken, trackid, time, id, classid):
  pass
  
def object_moved(recordingtoken, trackid, time, id, classid, confidence, x, y, width, height):
  pass
  