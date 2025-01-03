using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using UnityEngine;

namespace GISB.Runtime
{
    [CreateAssetMenu(fileName = "New GISB Bank", menuName = "GISB/Bank")]
    public class GISB_Bank : GISB_Playable
    {
        [JsonProperty(Order = 2)]
        public List<GISB_Event> events;

        public override GISB_Event GetEvent(string eventName)
        {
            foreach (GISB_Event gisbEvent in events)
            {
                if (gisbEvent.name == eventName)
                {
                    return gisbEvent;
                }
            }
            return null;
        }
    }
}

