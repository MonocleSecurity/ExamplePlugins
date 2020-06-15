function write_file()
  file = io.open("lua.txt", "w+")
  --LatLong
  latlong = get_latlong();
  if (latlong.latitude ~= nil and latlong.longitude ~= nil) then
    file:write("latitude:", latlong.latitude, " longitude:", latlong.longitude, "\n");
  end
  --Recordings
  recordings = get_recordings()
  for j=1,#recordings do
    recording = recordings[j]
    file:write("Recording: token:", recording.token, " name:", recording.name, " location:", recording.location, " retention_time:", recording.retention_time, " adaptive_streaming:", recording.adaptive_streaming == true and "true" or "false", " relative_location:", recording.relative_location == true and "true" or "false", " orientation:", recording.orientation)
    if (recording.latlong ~= nil) then
      file:write(" latitude:", recording.latlong.latitude, " longitude:", recording.latlong.longitude)
    end
    file:write("\n")
    for k=1,#recording.tracks do
      track = recording.tracks[k]
      parameters = ""
      for l=1,#track.parameters do
        parameters = parameters .. track.parameters[l]
      end
      active_parameters = "";
      for l=1,#track.active_parameters do
        active_parameters = active_parameters .. track.active_parameters[l]
      end
      file:write("  Track: id:", track.id, " track_type:", track.track_type, " description:", track.description, " state:", track.state, " error:", track.error, " mediauri:", track.mediauri, " parameters:", parameters, " active_parameters:", active_parameters, "\n")
    end
  end
  file:close();
end

function monocle_init(parameters)
  write_file()
end

function monocle_destroy()
  print("destroying")
end

function recording_added(recording)
  write_file()
end

function recording_changed(recording)
  write_file()
end

function recording_removed(token)
  write_file()
end

function latlong_changed(latlong)
  write_file()
end

function object_entered(recordingtoken, trackid, time, id, classid, confidence, x, y, width, height)
  print("object entered")
end

function object_exited(recordingtoken, trackid, time, id, classid)
  print("object exited")
end

function object_moved(recordingtoken, trackid, time, id, classid, confidence, x, y, width, height)
  print("object moved")
end
