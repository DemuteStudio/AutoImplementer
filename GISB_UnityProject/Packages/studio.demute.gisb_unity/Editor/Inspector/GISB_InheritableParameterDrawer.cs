using System.Collections;
using System.Collections.Generic;
using GISB.Runtime;
using UnityEditor;
using UnityEngine;

namespace GISB.Editor
{
    [CustomPropertyDrawer(typeof(GISB_InheritableParameter<>), true)]
    public class GISB_InheritableParameterDrawer : PropertyDrawer
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
                
                Rect headerRect = position;
                headerRect.height = baseHeight;
                property.isExpanded = EditorGUI.Foldout(headerRect, property.isExpanded, label, true);
                
                if (property.isExpanded)
                {
                    Rect propertyPosition = headerRect;
                    propertyPosition.y += baseHeight;

                    using (new EditorGUI.IndentLevelScope())
                    {
                        SerializedProperty parentParameter = GetParentParameter(property);
                        bool isOverriden = true;
                        if(parentParameter != null)
                        {
                            EditorGUI.PropertyField(propertyPosition, property.FindPropertyRelative("overrideParent"));
                            isOverriden = property.FindPropertyRelative("overrideParent").boolValue;
                        }
                        else
                        {
                            GUI.enabled = false;
                            EditorGUI.PropertyField(propertyPosition, property.FindPropertyRelative("overrideParent"));
                            GUI.enabled = true;
                        }
                        propertyPosition.y += baseHeight;

                        if (isOverriden)
                        {
                            SerializedProperty valueProperty = property.FindPropertyRelative("value");
                            DrawChildren(valueProperty, propertyPosition);
                        }
                        else
                        {
                            GUI.enabled = false;
                            while(!parentParameter.FindPropertyRelative("overrideParent").boolValue)
                            {
                                SerializedProperty lastParentParameter = parentParameter;
                                parentParameter = GetParentParameter(parentParameter);
                                if(parentParameter == null)
                                {
                                    parentParameter = lastParentParameter;
                                    break;
                                }
                            }
                            SerializedProperty valueProperty = parentParameter.FindPropertyRelative("value");
                            DrawChildren(valueProperty, propertyPosition);
                            GUI.enabled = true;
                        }
                    }
                }
            }
        }
        
        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            float baseHeight = GetBaseHeight();
            float height = baseHeight;
            
            if (property.isExpanded)
            {
                height += baseHeight;
                height += GetChildrenHeight(property.FindPropertyRelative("value"));
            }
            
            return height;
        }
        
        private static SerializedProperty GetParentParameter(SerializedProperty property)
        {
            string propertyPath = property.propertyPath;
            string audioObjectPath = propertyPath.Substring(0, propertyPath.LastIndexOf('.'));

            while (audioObjectPath.Contains("."))
            {
                string parentPath = audioObjectPath.Substring(0, audioObjectPath.LastIndexOf('.'));
                SerializedProperty parentProperty = property.serializedObject.FindProperty(parentPath);
                if (parentProperty == null) break;
                SerializedProperty parentParameterProperty = parentProperty.FindPropertyRelative(property.name);
                if (parentParameterProperty != null)
                {
                    return parentParameterProperty;
                }
                audioObjectPath = parentPath;
            }

            return null;
        }
        
        private static void DrawChildren(SerializedProperty property, Rect position)
        {
            int depth = property.depth;
            
            SerializedProperty iterator = property.Copy();
            while(iterator.NextVisible(true) && iterator.depth > depth)
            {
                position.height = EditorGUI.GetPropertyHeight(iterator);
                EditorGUI.PropertyField(position, iterator, true);
                position.y += position.height;
            }
        }
        
        private static float GetChildrenHeight(SerializedProperty property)
        {
            float height = 0;
            
            int depth = property.depth;
            
            SerializedProperty iterator = property.Copy();
            while(iterator.NextVisible(true) && iterator.depth > depth)
            {
                height += EditorGUI.GetPropertyHeight(iterator);
            }
            
            return height;
        }
    }
}

