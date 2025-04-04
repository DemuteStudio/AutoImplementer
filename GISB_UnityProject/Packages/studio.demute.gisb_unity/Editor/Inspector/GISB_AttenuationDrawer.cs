using System.Collections;
using System.Collections.Generic;
using GISB.Runtime;
using UnityEditor;
using UnityEngine;


namespace GISB.Editor
{
    [CustomPropertyDrawer(typeof(GISB_Attenuation), true)]
    public class GISB_AttenuationDrawer : PropertyDrawer
    {
        private const float curveHeightMultiplier = 4f;

        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            using (new EditorGUI.PropertyScope(position, label, property))
            {
                float baseHeight = GUI.skin.textField.CalcSize(GUIContent.none).y;

                Rect headerRect = position;
                headerRect.height = baseHeight;
                //property.isExpanded = EditorGUI.Foldout(headerRect, property.isExpanded, label, true);

                Rect propertyPosition = headerRect;

                //Draw basic properties until we hit the attenuation curve
                    int depth = property.depth;
                    SerializedProperty iterator = property.Copy();
                    while (iterator.NextVisible(true) && iterator.depth > depth)
                    {
                        if (iterator.name == "curve")
                        {
                            propertyPosition.height = baseHeight * curveHeightMultiplier;
                            //Draw the attenuation curve
                            GISB_Attenuation.AttenuationPreset preset =
                                (GISB_Attenuation.AttenuationPreset)property.FindPropertyRelative("preset")
                                    .enumValueIndex;
                            AnimationCurve curve = AnimationCurve.Linear(0, 1, 1, 0);
                            bool wasEnabled = GUI.enabled;
                            switch (preset)
                            {
                                case GISB_Attenuation.AttenuationPreset.Linear:
                                    GUI.enabled = false;
                                    //Create a new curve for the linear preset
                                    EditorGUI.CurveField(propertyPosition, "Curve", curve);
                                    GUI.enabled = wasEnabled;
                                    break;
                                case GISB_Attenuation.AttenuationPreset.Logarithmic:
                                    GUI.enabled = false;
                                    //Create a new curve for the logarithmic preset
                                    curve = GISB_Attenuation.GetAttenuationCurveForPreset(GISB_Attenuation
                                        .AttenuationPreset.Logarithmic);
                                    EditorGUI.CurveField(propertyPosition, "Curve", curve);
                                    GUI.enabled = wasEnabled;
                                    break;
                                case GISB_Attenuation.AttenuationPreset.Inverse:
                                    GUI.enabled = false;
                                    //Create a new curve for the inverse preset
                                    curve = GISB_Attenuation.GetAttenuationCurveForPreset(GISB_Attenuation
                                        .AttenuationPreset.Inverse);
                                    EditorGUI.CurveField(propertyPosition, "Curve", curve);
                                    GUI.enabled = wasEnabled;
                                    break;
                                case GISB_Attenuation.AttenuationPreset.Custom:
                                    EditorGUI.PropertyField(propertyPosition, iterator);
                                    break;
                                default:
                                    break;
                            }

                            propertyPosition.height = baseHeight;
                            propertyPosition.y += baseHeight * curveHeightMultiplier;
                        }
                        else
                        {
                            EditorGUI.PropertyField(propertyPosition, iterator);
                            propertyPosition.y += baseHeight;
                        }
                    }
            }
        }

        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            float baseHeight = GUI.skin.textField.CalcSize(GUIContent.none).y;
            float height = baseHeight;

            //Calculate the height of the properties
            int depth = property.depth;
            SerializedProperty iterator = property.Copy();
            while (iterator.NextVisible(true) && iterator.depth > depth)
            {
                if (iterator.name == "curve")
                {
                    //Add the height of the curve
                    height += baseHeight * curveHeightMultiplier;
                }
                else
                {
                    height += baseHeight;
                }
            }

            return height;
        }
    }
}