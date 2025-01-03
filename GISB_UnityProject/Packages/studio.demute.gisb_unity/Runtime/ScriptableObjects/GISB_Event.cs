using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    [CreateAssetMenu(fileName = "New GISB Event", menuName = "GISB/Event")]
    public class GISB_Event : ScriptableObject
    {
        [SerializeReference, SR, JsonProperty(Order = 2)]
        public GISB_AudioObjectBase rootAudioObject;
        
        public GISB_AudioObjectPlayer GetPlayer()
        {
            return rootAudioObject.GetPlayer();
        }

        [ExecuteInEditMode]
        public void PlayInEditor()
        {

        }
    }
}

