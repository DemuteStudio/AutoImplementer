using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using GISB.Runtime;
using Newtonsoft.Json;
using UnityEditor;
using UnityEngine;

namespace GISB.Editor
{
    [CustomEditor(typeof(GISB_Event))]
    public class GISB_EventEditor : UnityEditor.Editor
    {
        private GISB_AudioComponent editorAudioComponent;
        private Vector2 scrollPosition;
        
        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();
        
            GISB_Event gisbEvent = (GISB_Event)target;

            if(GUILayout.Button("Export to JSON"))
            {
                string path = EditorUtility.SaveFilePanel("Save GISB Event", Application.dataPath, target.name, "json");
                if (path.Length != 0)
                {
                    string folderPath = Path.GetDirectoryName(path);
                    JsonSerializerSettings settings = new JsonSerializerSettings
                    {
                        TypeNameHandling = TypeNameHandling.Auto,
                        Converters = new List<JsonConverter> { new GISB_AudioClipJsonConverter(folderPath) }
                    };
                    string json = JsonConvert.SerializeObject(gisbEvent, Formatting.Indented, settings);
                    System.IO.File.WriteAllText(path, json);
                }
            }

            if (GUILayout.Button("Import from JSON"))
            {
                string path = EditorUtility.OpenFilePanel("Import GISB Event", Application.dataPath, "json");
                if(path.Length != 0)
                {
                    string folderPath = Path.GetDirectoryName(path);
                    JsonSerializerSettings settings = new JsonSerializerSettings
                    {
                        TypeNameHandling = TypeNameHandling.Auto,
                        Converters = new List<JsonConverter> { new GISB_AudioClipJsonConverter(folderPath) }
                    };
                    string json = System.IO.File.ReadAllText(path);
                    JsonConvert.PopulateObject(json, target, settings);
                    EditorUtility.SetDirty(target);
                    //Auto save asset
                    AssetDatabase.SaveAssetIfDirty(target);
                }
            }
            
            EditorGUILayout.Space();

            if (editorAudioComponent == null)
            {
                GameObject editorPreviewObject = new GameObject("Editor Preview");
                editorPreviewObject.hideFlags = HideFlags.HideAndDontSave;
                editorAudioComponent = editorPreviewObject.AddComponent<GISB_AudioComponent>();
            }
            
            if (GUILayout.Button("Play"))
            {
                editorAudioComponent.PlayEvent(gisbEvent);
            }

            scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);
            Dictionary<string, List<string>> parameters = gisbEvent.rootAudioObject.ExtractParameters();
            foreach (KeyValuePair<string, List<string>> parameter in parameters)
            {
                EditorGUILayout.BeginHorizontal();
                EditorGUILayout.LabelField(parameter.Key);
                //Make dropdown for each possible value
                string[] values = parameter.Value.ToArray();
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
                EditorGUILayout.EndHorizontal();
            }
            EditorGUILayout.EndScrollView();

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

