using System;
using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using UnityEngine;

namespace GISB.Runtime
{
    [Serializable]
    public abstract class GISB_AudioObjectBase
    {
        //Data
        public GISB_AttenuationParameter attenuation = new GISB_AttenuationParameter();
        public GISB_VolumeParameter volume = new GISB_VolumeParameter();
        
        //Runtime Functions
        public abstract GISB_BaseAudioPlayer GetPlayer(GISB_BaseAudioPlayer parent = null);
        
        //Authoring Functions
        public abstract Dictionary<string, List<string>> ExtractParameters();
        public abstract float ExtractMaxDistance();
    }
}
