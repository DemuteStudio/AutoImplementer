using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Serialization;

namespace GISB.Runtime
{
    [System.Serializable]
    public abstract class GISB_RandomizableParameter<T> : GISB_Parameter
    {
        public bool randomize = false;
        public T minModifier;
        public T maxModifier;
        public T baseValue;
        
        public abstract T GetValue();
    }
}

