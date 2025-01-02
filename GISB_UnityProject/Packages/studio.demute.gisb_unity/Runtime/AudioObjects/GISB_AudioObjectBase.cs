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
        public abstract GISB_AudioObjectPlayer GetPlayer();
        
        public abstract Dictionary<string, List<string>> ExtractParameters();
    }
}
