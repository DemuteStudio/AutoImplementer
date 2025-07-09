using System;
using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using SerializeReferenceEditor;
using UnityEngine;
using UnityEngine.Serialization;

namespace GISB.Runtime
{
    [Serializable]
    public abstract class GISB_AudioObjectBase
    {
        //Data
        public GISB_AttenuationParameter attenuation = new GISB_AttenuationParameter();
        public GISB_VolumeParameter volumeDB = new GISB_VolumeParameter();
        public GISB_PitchParameter pitchCents = new GISB_PitchParameter();
        public GISB_LowpassParameter lowpassPercent = new GISB_LowpassParameter();
        public float playbackProbabilityPercent = 100.0f;
        
        //Runtime Functions
        public abstract GISB_BaseAudioPlayer GetPlayer(GISB_BaseAudioPlayer parent = null);
        
        //Authoring Functions
        public abstract Dictionary<string, List<string>> ExtractParameters();
        public abstract float ExtractMaxDistance();
    }

    public class GISB_AudioDropAttribute : PropertyAttribute
    {
        public string listName;

        public GISB_AudioDropAttribute(string listName)
        {
            this.listName = listName;
        }
    }
}
