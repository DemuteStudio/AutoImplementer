using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    [System.Serializable]
    public abstract class GISB_InheritableParameter<T>: GISB_Parameter
    {
        //Data
        public bool overrideParent = false;
        public T value;
    }
}

