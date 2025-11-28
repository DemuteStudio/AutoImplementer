using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using System;
using GISB.Runtime;
using SerializeReferenceEditor.Editor;

namespace GISB.Editor
{
    [CustomPropertyDrawer(typeof(GISB_AudioDropAttribute))]
    public class GISB_AudioDropAttributeDrawer : PropertyDrawer
    {
        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            EditorGUI.HelpBox(position, "Drag and drop AudioClip assets here to add them to the playlist.", MessageType.Info);
            GISB_AudioDropAttribute audioDropAttribute = (GISB_AudioDropAttribute)attribute;
            
            SerializedProperty arrayProperty = property.serializedObject.FindProperty(property.propertyPath.Substring(0, property.propertyPath.LastIndexOf('.'))+ $".{audioDropAttribute.listName}");

            if (position.Contains(Event.current.mousePosition))
            {
                switch (Event.current.type)
                {
                    case EventType.DragUpdated:
                        DragAndDrop.visualMode = DragAndDropVisualMode.Copy;
                        Event.current.Use();
                        break;
                    case EventType.DragPerform:
                        DragAndDrop.AcceptDrag();
                        foreach (object obj in DragAndDrop.objectReferences)
                        {
                            if (obj is AudioClip audioClip)
                            {
                                int index = arrayProperty.arraySize;
                                arrayProperty.InsertArrayElementAtIndex(index);
                                arrayProperty.GetArrayElementAtIndex(index).managedReferenceValue = new GISB_SingleSound()
                                {
                                    soundClip = audioClip
                                };
                            }
                        }
                        property.serializedObject.ApplyModifiedProperties();
                        Event.current.Use();
                        break;
                }
            }
        }
        
        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            return EditorGUIUtility.singleLineHeight * 2; // Adjust height as needed
        }
    }
}


