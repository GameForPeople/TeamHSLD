using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LoadingManager : MonoBehaviour
{
    public LoadingInfo[] loadingInfo;

    public Text loadingText;
    public Image loadingImg;
    public Slider loadingBar;
    public Text loadingMent;

    IEnumerator LoadingTextCor()
    {
        loadingText.text = "Loading";
        yield return new WaitForSeconds(0.5f);
        loadingText.text = "Loading .";
        yield return new WaitForSeconds(0.5f);
        loadingText.text = "Loading ..";
        yield return new WaitForSeconds(0.5f);
        loadingText.text = "Loading ...";
        yield return new WaitForSeconds(0.5f);
        StartCoroutine(LoadingTextCor());
    }

    private void OnEnable()
    {
        StartCoroutine(LoadingTextCor());
        SetLoading(Random.Range(0,6));
    }

    private void OnDisable()
    {
        StopAllCoroutines();
    }

    public void SetLoading(int index)
    {
        loadingMent.text = loadingInfo[index].ment;
        loadingImg.sprite = loadingInfo[index].spr;
    }

}
