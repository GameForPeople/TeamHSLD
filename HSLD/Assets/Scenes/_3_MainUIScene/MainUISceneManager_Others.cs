using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    private ParticleSystem characterSuccessParticle;
    private ParticleSystem characterFailParticle;
    
    // Use this for initialization
    void StartForOthers()
    {
        // DEV_77에 의해, CoreUI에서 MainUIScene으로 이동함.

        // GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(true);
        // GameObject.Find("GameCores").transform.Find("UserDataUI").transform.Find("Canvas_Dynamic").transform.Find("Money_Text").gameObject.GetComponent<Text>().text
        //     = networkObject.money.ToString();
        //
        // GameObject.Find("GameCores").transform.Find("UserDataUI").transform.Find("Canvas").transform.Find("NickName_Text").gameObject.GetComponent<Text>().text
        //     = networkObject.nickName.ToString();

        characterSuccessParticle = GameObject.Find("FX_Canvas").transform.Find("CharacterSuccessParticle").GetComponent<ParticleSystem>();
        characterSuccessParticle.Stop();

        characterFailParticle = GameObject.Find("FX_Canvas").transform.Find("CharacterFailParticle").GetComponent<ParticleSystem>();
        characterFailParticle.Stop();
    }

    public void PlayParticle(int InParticleIndex)
    {
        if(InParticleIndex == 1)
            StartCoroutine(PlayCharacterSuccessParticleCoroutine());
        else if (InParticleIndex == 2)
            StartCoroutine(PlayCharacterFailParticleCoroutine());
    }

    IEnumerator PlayCharacterSuccessParticleCoroutine()
    {
        characterSuccessParticle.Play();
        yield return new WaitForSeconds(1.0f);
    }

    IEnumerator PlayCharacterFailParticleCoroutine()
    {
        characterFailParticle.Play();
        yield return new WaitForSeconds(1.0f);
    }
}
