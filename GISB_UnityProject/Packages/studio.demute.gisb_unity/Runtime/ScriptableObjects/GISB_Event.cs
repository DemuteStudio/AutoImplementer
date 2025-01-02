using System.Collections;
using System.Collections.Generic;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    [CreateAssetMenu(fileName = "New GISB Event", menuName = "GISB/Event")]
    public class GISB_Event : ScriptableObject
    {
        [SerializeReference, SR]
        public GISB_AudioObjectBase rootAudioObject;
    }
}

