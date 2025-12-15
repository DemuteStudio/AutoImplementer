using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GISB.Runtime;
using UnityEditor;


namespace GISB.Editor
{ 
    [ExecuteInEditMode]
    public class GISB_EditorPreviewComponent : GISB_AudioComponent
    {
        public void OnEnable()
        {
            EditorApplication.update += EditorUpdate;
        }
        
        public void OnDisable()
        {
            EditorApplication.update -= EditorUpdate;
        }

        public void EditorUpdate()
        {
            foreach (GISB_EventInstance eventInstance in activeEventInstances)
            {
                eventInstance.UpdateGameThread(Time.deltaTime);
                //Debug.Log("Updating GISB Editor Preview Component");
            }
        }
    }
}