using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    [System.Serializable]
    public class GISB_AttenuationParameter : GISB_InheritableParameter<GISB_Attenuation> { }

    [System.Serializable]
    public class GISB_Attenuation
    {
        public bool active = false;
        public float minDistance = 1;
        public float maxDistance = 10;
        public AnimationCurve attenuationCurve = AnimationCurve.Linear(0, 1, 1, 0);
    }
}

