using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using UnityEngine;

namespace GISB.Runtime
{
    [CreateAssetMenu(fileName = "New GISB Bank", menuName = "GISB/Bank")]
    public class GISB_Bank : GISB_Asset
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

        public override List<GISB_Event> GetAllEvents()
        {
            return events;
        }

        public override Dictionary<string, List<string>> ExtractParameters()
        {
            Dictionary<string, List<string>> parameters = new Dictionary<string, List<string>>();
            foreach (GISB_Event gisbEvent in events)
            {
                if(gisbEvent == null)
                {
                    continue;
                }
                Dictionary<string, List<string>> extractedParameters = gisbEvent.ExtractParameters();
                foreach (KeyValuePair<string, List<string>> extractedParameter in extractedParameters)
                {
                    if (!parameters.ContainsKey(extractedParameter.Key))
                    {
                        parameters.Add(extractedParameter.Key, new List<string>());
                    }
                    parameters[extractedParameter.Key].AddRange(extractedParameter.Value);
                }
            }
            return parameters;
        }
    }
}

