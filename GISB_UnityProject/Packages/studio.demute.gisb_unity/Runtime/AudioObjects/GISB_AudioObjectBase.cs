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
        
        //Runtime Functions
        public abstract GISB_AudioObjectPlayer GetPlayer();
        
        //Authoring Functions
        public abstract Dictionary<string, List<string>> ExtractParameters();
    }
}
