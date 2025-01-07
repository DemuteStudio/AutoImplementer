using System.Collections;
using System.Collections.Generic;
using GISB.Runtime;
using UnityEditor;
using UnityEngine;

namespace GISB.Editor
{
    [CustomPropertyDrawer(typeof(GISB_Asset))]
    public class GISB_AssetDrawer : PropertyDrawer
    {
        private static float GetBaseHeight()
        {
            return GUI.skin.textField.CalcSize(GUIContent.none).y;
        }

        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            using (new EditorGUI.PropertyScope(position, label, property))
            {
                float baseHeight = GetBaseHeight();

                GISB_Asset gisbAsset = property.objectReferenceValue as GISB_Asset;
                
                Rect headerRect = position;
                headerRect.width = EditorGUIUtility.labelWidth;
                headerRect.height = baseHeight;
                
                property.isExpanded = EditorGUI.Foldout(headerRect, property.isExpanded, label, true);

                
                Rect searchRect = new Rect(position.xMax - 100, position.y, 100, baseHeight);
                Rect pathRect = position;
                pathRect.xMin = headerRect.xMax;
                pathRect.xMax = searchRect.x - 3;
                pathRect.height = baseHeight;

                EditorGUI.PropertyField(pathRect, property, GUIContent.none);
                
                if (GUI.Button(searchRect, new GUIContent("Search")))
                {
                    var eventBrowser = ScriptableObject.CreateInstance<GISB_BankBrowser>();

                    eventBrowser.ChooseEvent(property);
                    var windowRect = position;
                    windowRect.xMin = pathRect.xMin;
                    windowRect.position = GUIUtility.GUIToScreenPoint(windowRect.position);
                    windowRect.height = searchRect.height + 1;
                    windowRect.width = Mathf.Max(windowRect.width, 300f);
                    eventBrowser.ShowAsDropDown(windowRect, new Vector2(windowRect.width, 400));
                }
                
                if (property.isExpanded && gisbAsset)
                {
                    float labelY = headerRect.y + baseHeight;
                    
                    using (new EditorGUI.IndentLevelScope())
                    {
                        Rect labelRect = EditorGUI.IndentedRect(headerRect);
                        labelRect.y = labelY;

                        Rect valueRect = labelRect;
                        valueRect.xMin = labelRect.xMax;
                        valueRect.width = labelRect.width;

                        Rect subValueRect = valueRect;
                        subValueRect.xMin = valueRect.xMax;
                        subValueRect.xMax = position.xMax;
                        
                        GUI.Label(labelRect, new GUIContent("Events"), EditorStyles.boldLabel);
                        labelRect.y += baseHeight;
                        valueRect.y += baseHeight;
                        subValueRect.y += baseHeight;
                        
                        //Get all events in the bank
                        List<GISB_Event> events = gisbAsset.GetAllEvents();
                        foreach (GISB_Event gisbEvent in events)
                        {
                            GUI.Label(valueRect, new GUIContent(gisbEvent.name));
                            labelRect.y += baseHeight;
                            valueRect.y += baseHeight;
                            subValueRect.y += baseHeight;
                        }
                        
                        GUI.Label(labelRect, new GUIContent("Parameters"), EditorStyles.boldLabel);
                        labelRect.y += baseHeight;
                        valueRect.y += baseHeight;
                        subValueRect.y += baseHeight;
                        
                        //Get all parameters in the bank
                        Dictionary<string, List<string>> parameters = gisbAsset.ExtractParameters();
                        foreach (KeyValuePair<string, List<string>> parameter in parameters)
                        {
                            GUI.Label(valueRect, new GUIContent(parameter.Key));

                            foreach (string value in parameter.Value)
                            {
                                GUI.Label(subValueRect, new GUIContent(value));
                                labelRect.y += baseHeight;
                                valueRect.y += baseHeight;
                                subValueRect.y += baseHeight;
                            }
                            
                            labelRect.y += baseHeight;
                            valueRect.y += baseHeight;
                            subValueRect.y += baseHeight;
                        }
                    }
                }
            }
        }
        
        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            float baseHeight = GetBaseHeight();

            float height;

            if (property.isExpanded)
            {
                height = baseHeight * 3;
                GISB_Asset gisbAsset = property.objectReferenceValue as GISB_Asset;
                if (gisbAsset)
                {
                    height += baseHeight * (gisbAsset.GetAllEvents().Count);
                    Dictionary<string, List<string>> parameters = gisbAsset.ExtractParameters();
                    foreach (KeyValuePair<string,List<string>> parameter in parameters)
                    {
                        height += baseHeight * (parameter.Value.Count + 1);
                    }
                }
            }
            else
            {
                height = baseHeight;
            }

            return height;
        }
    }
}

