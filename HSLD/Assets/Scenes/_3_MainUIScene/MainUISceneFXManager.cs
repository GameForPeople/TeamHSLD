using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainUISceneFXManager : MonoBehaviour {

    public ParticleSystem successParticle;
    public ParticleSystem failParticle;

    // Use this for initialization
    void Start () {

	}

    public void RenderSuccessOrFailParticle(bool InisSuccessOrFail)
    {
        if (InisSuccessOrFail)
        {
            //Instantiate(successParticle, transform.position, Quaternion.identity);
            //GameObject.Find("FX_Canvas").transform.Find("SuccessParticle").gameObject.SetActive(true);
            successParticle.Play();
        }
        else
        {
            failParticle.Play();
            //GameObject.Find("FX_Canvas").transform.Find("FailParticle").gameObject.SetActive(true);
        }
    }
}
