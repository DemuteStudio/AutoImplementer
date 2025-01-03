using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    [CreateAssetMenu(fileName = "New GISB Event", menuName = "GISB/Event")]
    public class GISB_Event : GISB_Asset
    {
        [SerializeReference, SR, JsonProperty(Order = 2)]
        public GISB_AudioObjectBase rootAudioObject;
        
        public GISB_AudioObjectPlayer GetPlayer()
        {
            return rootAudioObject.GetPlayer();
        }

        public override GISB_Event GetEvent(string eventName)
        {
            return this;
        }
    }
}

