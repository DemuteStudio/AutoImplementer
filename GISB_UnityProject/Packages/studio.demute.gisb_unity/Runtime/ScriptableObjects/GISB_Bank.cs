using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using UnityEngine;

namespace GISB.Runtime
{
    [CreateAssetMenu(fileName = "New GISB Bank", menuName = "GISB/Bank")]
    public class GISB_Bank : ScriptableObject
    {
        [JsonProperty(Order = 2)]
        public List<GISB_Event> events;
    }
}

