using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    [SRName("Switch Sound")]
    public class GISB_SwitchSound : GISB_AudioObjectBase
    {
        [System.Serializable]
        public struct SwitchCase
        {
            public string parameterValue;
            [SerializeReference, SR]
            public GISB_AudioObjectBase audioObject;
        }
        
        public string ParameterName;
        public string DefaultValue;
        public SwitchCase[] SwitchCases;

        public override GISB_AudioObjectPlayer GetPlayer()
        {
            return new GISB_SwitchSoundPlayer(this);
        }

        public override Dictionary<string, List<string>> ExtractParameters()
        {
            Dictionary<string, List<string>> parameters = new Dictionary<string, List<string>>();
            parameters.Add(ParameterName, new List<string>());
            foreach (SwitchCase switchCase in SwitchCases)
            {
                parameters[ParameterName].Add(switchCase.parameterValue);
                if (switchCase.audioObject != null)
                {
                    Dictionary<string, List<string>> extractedParameters = switchCase.audioObject.ExtractParameters();
                    foreach (KeyValuePair<string, List<string>> extractedParameter in extractedParameters)
                    {
                        if (!parameters.ContainsKey(extractedParameter.Key))
                        {
                            parameters.Add(extractedParameter.Key, new List<string>());
                        }
                        parameters[extractedParameter.Key].AddRange(extractedParameter.Value);
                    }
                }
            }
            return parameters;
        }
    }
}
