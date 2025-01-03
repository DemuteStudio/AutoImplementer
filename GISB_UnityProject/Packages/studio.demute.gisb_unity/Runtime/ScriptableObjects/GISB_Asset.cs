using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public abstract class GISB_Asset : ScriptableObject
    {
        public abstract GISB_Event GetEvent(string eventName);
    }
}

