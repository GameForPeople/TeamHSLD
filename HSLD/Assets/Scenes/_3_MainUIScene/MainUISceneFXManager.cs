using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainUISceneFXManager : MonoBehaviour {

    public ParticleSystem successParticle;
    public ParticleSystem failParticle;

    // Use this for initialization
    void Start () {
        GameObject.Find("FX_Canvas").GetComponent<Canvas>().worldCamera 
            = GameObject.Find("GameCores").transform.Find("ClientBase_Space").transform.Find("ClientBase_Camera").GetComponent<Camera>();
	}

    public void RenderSuccessOrFailParticle(bool InisSuccessOrFail)
    {
        if (InisSuccessOrFail)
        {
            //Instantiate(successParticle, transform.position, Quaternion.identity);
            GameObject.Find("FX_Canvas").transform.Find("SuccessParticle").gameObject.SetActive(true);
        }
        else
        {
            GameObject.Find("FX_Canvas").transform.Find("FailParticle").gameObject.SetActive(true);
        }
    }
}
