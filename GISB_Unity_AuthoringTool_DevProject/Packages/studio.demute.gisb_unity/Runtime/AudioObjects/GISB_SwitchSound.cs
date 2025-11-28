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

        public override GISB_BaseAudioPlayer GetPlayer(GISB_BaseAudioPlayer parent = null)
        {
            return new GISB_SwitchSoundPlayer(this, parent);
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
        
        public override List<string> ExtractFloatParameters()
        {
            List<string> floatParameters = new List<string>();
            foreach (SwitchCase switchCase in SwitchCases)
            {
                if (switchCase.audioObject != null)
                {
                    List<string> extractedFloatParameters = switchCase.audioObject.ExtractFloatParameters();
                    foreach (string floatParameter in extractedFloatParameters)
                    {
                        if (!floatParameters.Contains(floatParameter))
                        {
                            floatParameters.Add(floatParameter);
                        }
                    }
                }
            }

            foreach (GISB_Modulator modulator in modulators)
            {
                if (!floatParameters.Contains(modulator.ParameterName))
                {
                    floatParameters.Add(modulator.ParameterName);
                }            
            }
            return floatParameters;
        }

        public override float ExtractMaxDistance()
        {
            float maxDistance = attenuation.overrideParent && attenuation.value != null && attenuation.value.active ? attenuation.value.maxDistance : 0f;
            foreach (SwitchCase switchCase in SwitchCases)
            {
                if (switchCase.audioObject == null)
                {
                    continue;
                }
                float objectMaxDistance = switchCase.audioObject.ExtractMaxDistance();
                if (objectMaxDistance > maxDistance)
                {
                    maxDistance = objectMaxDistance;
                }
            }
            return maxDistance;
        }
    }
}
