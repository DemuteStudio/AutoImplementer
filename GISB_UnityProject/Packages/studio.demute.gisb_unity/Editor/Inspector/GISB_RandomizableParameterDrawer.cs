using System.Collections;
using System.Collections.Generic;
using GISB.Runtime;
using UnityEngine;
using UnityEditor;

namespace GISB.Editor
{
    [CustomPropertyDrawer(typeof(GISB_RandomizableParameter<>), true)]
    public class GISB_RandomizableParameterDrawer : PropertyDrawer
    {
        private static float GetBaseHeight()
        {
            return GUI.skin.textField.CalcSize(GUIContent.none).y;
        }
        
        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            // Draw the property field
            //EditorGUI.PropertyField(position, property, label, true);
            
            Rect headerRect = position;
            headerRect.height = GetBaseHeight();
            headerRect.width = EditorGUIUtility.labelWidth;
            
            property.isExpanded = EditorGUI.Foldout(headerRect, property.isExpanded, label, true);

            Rect baseValueRect = position;
            baseValueRect.xMin = headerRect.xMax;
            baseValueRect.height = GetBaseHeight();
            
            EditorGUI.PropertyField(baseValueRect, property.FindPropertyRelative("baseValue"), GUIContent.none);

            if (property.isExpanded)
            {
                float positionY = headerRect.y + GetBaseHeight();
                
                using (new EditorGUI.IndentLevelScope())
                {
                    Rect labelRect = EditorGUI.IndentedRect(headerRect);
                    labelRect.y = positionY;
                    
                    Rect toggleRect = labelRect;
                    toggleRect.x = labelRect.x - 20;
                    toggleRect.xMax = labelRect.xMin;
                    toggleRect.width = 20;
                    
                    Rect minModifierRect = labelRect;
                    minModifierRect.xMin = labelRect.xMax;
                    minModifierRect.width = baseValueRect.width/2;
                    
                    Rect maxModifierRect = minModifierRect;
                    maxModifierRect.xMin = minModifierRect.xMax;
                    maxModifierRect.width = minModifierRect.width;
                    
                    EditorGUI.LabelField(labelRect, "Randomize");
                    EditorGUI.PropertyField(toggleRect, property.FindPropertyRelative("randomize"), GUIContent.none);
                    
                    bool isRandomize = property.FindPropertyRelative("randomize").boolValue;
                    bool wasEnabled = GUI.enabled;
                    GUI.enabled = isRandomize;
                    EditorGUI.PropertyField(minModifierRect, property.FindPropertyRelative("minModifier"), GUIContent.none);
                    EditorGUI.PropertyField(maxModifierRect, property.FindPropertyRelative("maxModifier"), GUIContent.none);
                    GUI.enabled = wasEnabled;
                }
            }
        }
        
        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            if (property.isExpanded)
            {
                return GetBaseHeight() * 2;
            }
            else
            {
                return GetBaseHeight();
            }
        }
    }
}
