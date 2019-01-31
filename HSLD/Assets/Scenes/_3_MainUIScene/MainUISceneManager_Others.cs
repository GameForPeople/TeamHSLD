using System.Collections;
using System.Collections.Generic;

using System;

using UnityEngine;
using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    private ParticleSystem characterSuccessParticle;

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
    }

    public void PlayParticle(int InParticleIndex)
    {
        if(InParticleIndex == 1)
            StartCoroutine(PlayCharacterSuccessParticleCoroutine());
    }

    IEnumerator PlayCharacterSuccessParticleCoroutine()
    {
        characterSuccessParticle.Play();
        yield return new WaitForSeconds(1.0f);
    }

}
