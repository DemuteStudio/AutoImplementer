using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using UnityEditor;
using UnityEngine;

namespace GISB.Editor
{
    public class GISB_AudioClipJsonConverter : JsonConverter<AudioClip>
    {
        public string JsonDirectory { get; set; }
        
        public GISB_AudioClipJsonConverter(string jsonDirectory)
        {
            JsonDirectory = jsonDirectory;
        }
        
        public override void WriteJson(JsonWriter writer, AudioClip value, JsonSerializer serializer)
        {
            string filename = Path.GetFileName(AssetDatabase.GetAssetPath(value));

            writer.WriteValue(filename);
            
            string folderPath = JsonDirectory + "/Audio Files";
            //Copy the audio clip to the path of the written json
            if (!System.IO.Directory.Exists(folderPath))
            {
                System.IO.Directory.CreateDirectory(folderPath);
            }
            File.Copy(AssetDatabase.GetAssetPath(value), Path.Combine(folderPath, filename), true);
        }

        public override AudioClip ReadJson(JsonReader reader, Type objectType, AudioClip existingValue, bool hasExistingValue,
            JsonSerializer serializer)
        {
            string filename = (string)reader.Value;
            string path = JsonDirectory + "/Audio Files/" + filename;
            
            //Copy audio file into correct directory in project
            string audioFilesFolder = "Assets/GISB/Audio Files";
            if (!Directory.Exists(audioFilesFolder))
            {
                Directory.CreateDirectory(audioFilesFolder);
            }
            string assetPath = "Assets/GISB/Audio Files/" + filename;
            if (!File.Exists(assetPath))
            {
                File.Copy(path, assetPath);
                AssetDatabase.Refresh();
            }
            return AssetDatabase.LoadAssetAtPath<AudioClip>(assetPath);
        }
    }
}
