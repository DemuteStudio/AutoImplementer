using System.Collections;
using System.Collections.Generic;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_RandomSound : GISB_AudioObjectBase
    {
        [SerializeReference, SR]
        public GISB_AudioObjectBase[] RandomPlaylist;
    }
}
