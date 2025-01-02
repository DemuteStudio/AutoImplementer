using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
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
    }
}
