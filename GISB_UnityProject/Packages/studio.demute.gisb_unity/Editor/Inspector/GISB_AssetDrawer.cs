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
            }
        }
    }
}

