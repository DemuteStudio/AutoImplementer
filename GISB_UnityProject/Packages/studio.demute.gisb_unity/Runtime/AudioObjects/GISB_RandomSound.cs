using System.Collections;
using System.Collections.Generic;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_RandomSound : GISB_AudioObjectBase
    {
        public int avoidLastPlayed = 0;
        
        [SerializeReference, SR]
        public GISB_AudioObjectBase[] RandomPlaylist;
        
        public override GISB_AudioObjectPlayer GetPlayer()
        {
            return new GISB_RandomSoundPlayer(this);
        }

        public override Dictionary<string, List<string>> ExtractParameters()
        {
            Dictionary<string, List<string>> parameters = new Dictionary<string, List<string>>();
            foreach (GISB_AudioObjectBase audioObject in RandomPlaylist)
            {
                Dictionary<string, List<string>> extractedParameters = audioObject.ExtractParameters();
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
