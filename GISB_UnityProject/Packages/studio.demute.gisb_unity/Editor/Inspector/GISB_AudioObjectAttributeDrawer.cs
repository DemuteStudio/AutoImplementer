using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using System;
using GISB.Runtime;
using SerializeReferenceEditor.Editor;

namespace GISB.Editor
{
    [CustomPropertyDrawer(typeof(GISB_AudioListAttribute))]
    public class GISB_AudioObjectAttributeDrawer : PropertyDrawer
    {
        private SRDrawer _srDrawer = new();

        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            _srDrawer.Draw(position, property, label);
            
            SerializedProperty arrayProperty = property.serializedObject.FindProperty(property.propertyPath.Substring(0, property.propertyPath.LastIndexOf('.')));

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
                        bool firstElement = true;
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

                        break;
                }
            }
        }
        
        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            return _srDrawer.GetPropertyHeight(property, label);
        }
    }
}


