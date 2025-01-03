using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using GISB.Runtime;
using Newtonsoft.Json;
using UnityEditor;
using UnityEngine;
using UnityEngine.Networking;

namespace GISB.Editor
{
    public class GISB_AudioClipJsonConverter : JsonConverter<AudioClip>
    {
        public string JsonDirectory { get; set; }
        
        public bool WebURL { get; set; }
        
        public GISB_AudioClipJsonConverter(string jsonDirectory, bool webURL)
        {
            JsonDirectory = jsonDirectory;
            WebURL = webURL;
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
                if (!WebURL)
                {
                    File.Copy(path, assetPath);
                }
                else
                {
                    using (UnityWebRequest www = UnityWebRequest.Get(path))
                    {
                        www.downloadHandler = new DownloadHandlerFile(assetPath);
                        www.SendWebRequest();
                        while (!www.isDone)
                        {
                            EditorUtility.DisplayProgressBar("Downloading", path, www.downloadProgress);
                        }
                        EditorUtility.ClearProgressBar();
                        if (www.error != null)
                        {
                            Debug.LogError(www.error);
                        }
                    }
                }
                
                AssetDatabase.Refresh();
            }
            return AssetDatabase.LoadAssetAtPath<AudioClip>(assetPath);
        }
    }

    public class GISB_EventRefJsonConverter : JsonConverter<List<GISB_Event>>
    {
        public string JsonDirectory { get; set; }
        
        public bool WebURL { get; set; }
        public GISB_EventRefJsonConverter(string jsonDirectory, bool webURL)
        {
            JsonDirectory = jsonDirectory;
            WebURL = webURL;
        }
        public override void WriteJson(JsonWriter writer, List<GISB_Event> value, JsonSerializer serializer)
        {
            writer.WriteStartArray();
            foreach (GISB_Event gisbEvent in value)
            {
                writer.WriteValue(gisbEvent.name);
            }
            writer.WriteEndArray();
            
            string folderPath = JsonDirectory + "/Events";
            if(!Directory.Exists(folderPath))
            {
                Directory.CreateDirectory(folderPath);
            }
            foreach (GISB_Event gisbEvent in value)
            {
                GISB_EventEditor.ExportToJSON(folderPath + "/" + gisbEvent.name + ".json", gisbEvent);
            }
        }

        public override List<GISB_Event> ReadJson(JsonReader reader, Type objectType, List<GISB_Event> existingValue, bool hasExistingValue,
            JsonSerializer serializer)
        {
            List<GISB_Event> events = new List<GISB_Event>();
            while (reader.Read())
            {
                if (reader.TokenType == JsonToken.String)
                {
                    string filename = (string)reader.Value;
                    string path = JsonDirectory + "/Events/" + filename + ".json";
                    string eventAssetsFolder = "Assets/GISB/Events/";
                    if (!Directory.Exists(eventAssetsFolder))
                    {
                        Directory.CreateDirectory(eventAssetsFolder);
                    }
                    GISB_Event gisbEvent = AssetDatabase.LoadAssetAtPath<GISB_Event>(eventAssetsFolder + filename + ".asset");
                    if(gisbEvent == null)
                    {
                        gisbEvent = ScriptableObject.CreateInstance<GISB_Event>();
                        AssetDatabase.CreateAsset(gisbEvent, eventAssetsFolder + filename + ".asset");
                    }
                    GISB_EventEditor.ImportFromJSON(path, gisbEvent, WebURL);
                    AssetDatabase.SaveAssetIfDirty(gisbEvent);
                    events.Add(gisbEvent);
                }
                if(reader.TokenType == JsonToken.EndArray)
                {
                    break;
                }
            }
            return events;
        }
    }
}
