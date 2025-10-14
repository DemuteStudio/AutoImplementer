using System;
using UnityEngine;

namespace GISB.Runtime
{
    [Serializable]
    public class GISB_Modulator
    {
        public enum ModulatorTarget
        {
            Volume,
        }
        
        public ModulatorTarget Target;
        public string ParameterName;
        public AnimationCurve Curve = AnimationCurve.Linear(0, 0, 1, 0);
        
    }
}