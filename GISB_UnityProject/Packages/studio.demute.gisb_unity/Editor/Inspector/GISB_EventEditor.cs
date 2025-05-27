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
    [CustomEditor(typeof(GISB_Event))]
    public class GISB_EventEditor : UnityEditor.Editor
    {
        private GISB_AudioComponent editorAudioComponent;
        private Vector2 scrollPosition;
        private float distance = 0f;
        public override void OnInspectorGUI()
        {
            EditorGUI.BeginChangeCheck();
            base.OnInspectorGUI();
            if (EditorGUI.EndChangeCheck())
            {
                //Reload editor preview
                if (editorAudioComponent != null)
                {
                    DestroyImmediate(editorAudioComponent.gameObject);
                    editorAudioComponent = null;
                }
            }
            GISB_Event gisbEvent = (GISB_Event)target;

            if(GUILayout.Button("Export to JSON"))
            {
                string path = EditorUtility.SaveFilePanel("Save GISB Event", Application.dataPath, target.name, "json");
                if (path.Length != 0)
                {
                    ExportToJSON(path, gisbEvent);
                }
            }

            if (GUILayout.Button("Import from JSON"))
            {
                string path = EditorUtility.OpenFilePanel("Import GISB Event", Application.dataPath, "json");
                if(path.Length != 0)
                {
                    ImportFromJSON(path, gisbEvent, false);
                }
            }

            if (GUILayout.Button("Download from Server"))
            {
                string eventPath = GISB_EditorSettings.LoadOrDefault().JSONImportAddress + "/Events/" + gisbEvent.name + ".json";
                ImportFromJSON(eventPath, gisbEvent, true);
            }
            
            EditorGUILayout.Space();

            if (editorAudioComponent == null)
            {
                GameObject editorPreviewObject = new GameObject("Editor Preview");
                editorPreviewObject.hideFlags = HideFlags.DontSave;
                editorAudioComponent = editorPreviewObject.AddComponent<GISB_EditorPreviewComponent>();
            }
            
            if (GUILayout.Button("Play"))
            {
                editorAudioComponent.PlayEvent(gisbEvent);
            }
            
            if (GUILayout.Button("Stop"))
            {
                editorAudioComponent.StopEvent(gisbEvent);
            }
            
            float maxDistance = gisbEvent.rootAudioObject?.ExtractMaxDistance() ?? 0f;
            if(maxDistance > 0f)
            {
                distance = EditorGUILayout.Slider( "Distance", distance, 0f, maxDistance);
            }
            
            AudioListener audioListener = FindObjectOfType<AudioListener>();
            editorAudioComponent.transform.position = audioListener.transform.position + audioListener.transform.forward  * distance;

            if(gisbEvent.rootAudioObject != null)
            {
                scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);
                Dictionary<string, List<string>> parameters = gisbEvent.rootAudioObject.ExtractParameters();
                foreach (KeyValuePair<string, List<string>> parameter in parameters)
                {
                    EditorGUILayout.BeginHorizontal();
                    EditorGUILayout.LabelField(parameter.Key);
                    //Make dropdown for each possible value
                    string[] values = parameter.Value.ToArray();
                    if (values.Length > 0)
                    {
                        if(editorAudioComponent.activeParameters.ContainsKey(parameter.Key) == false)
                        {
                            editorAudioComponent.activeParameters.Add(parameter.Key, values[0]);
                        }
                        int selectedIndex = Array.IndexOf(values, editorAudioComponent.activeParameters[parameter.Key]);
                        int newSelectedIndex = EditorGUILayout.Popup(selectedIndex, values);
                        if (newSelectedIndex != selectedIndex)
                        {
                            editorAudioComponent.activeParameters[parameter.Key] = values[newSelectedIndex];
                        }
                    }
                    EditorGUILayout.EndHorizontal();
                }
                EditorGUILayout.EndScrollView();
            }
        }

        public static void ImportFromJSON(string path, GISB_Event gisbEvent, bool webUrl)
        {
            string folderPath = Path.GetDirectoryName(path);
            JsonSerializerSettings settings = new JsonSerializerSettings
            {
                TypeNameHandling = TypeNameHandling.Auto,
                Converters = new List<JsonConverter> { new GISB_AudioClipJsonConverter(folderPath, webUrl) }
            };
            string json = "";
            if (webUrl)
            {
                using (UnityWebRequest webRequest = UnityWebRequest.Get(path))
                {
                    webRequest.SendWebRequest();
                    while (!webRequest.isDone)
                    {
                        EditorUtility.DisplayProgressBar("Downloading", path, webRequest.downloadProgress);
                    }
                    EditorUtility.ClearProgressBar();
                    if (webRequest.error != null)
                    {
                        Debug.LogError(webRequest.error);
                        return;
                    }
                    json = webRequest.downloadHandler.text;
                }
            }
            else
            {
                json = System.IO.File.ReadAllText(path);
            }
            JsonConvert.PopulateObject(json, gisbEvent, settings);
            EditorUtility.SetDirty(gisbEvent);
            //Auto save asset
            AssetDatabase.SaveAssetIfDirty(gisbEvent);
        }

        public static void ExportToJSON(string path, GISB_Event gisbEvent)
        {
            string folderPath = Path.GetDirectoryName(path);
            JsonSerializerSettings settings = new JsonSerializerSettings
            {
                TypeNameHandling = TypeNameHandling.Auto,
                Converters = new List<JsonConverter> { new GISB_AudioClipJsonConverter(folderPath, false) }
            };
            string json = JsonConvert.SerializeObject(gisbEvent, Formatting.Indented, settings);
            System.IO.File.WriteAllText(path, json);
        }


        public void OnDestroy()
        {
            if (editorAudioComponent != null)
            {
                DestroyImmediate(editorAudioComponent.gameObject);
            }
        }
    }
}

