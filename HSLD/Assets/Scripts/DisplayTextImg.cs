using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DisplayTextImg : MonoBehaviour
{
    public Sprite[] sprs;
    

    public void Performance(Sprite spr)
    {
        if (spr.Equals(sprs[0]))
            gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(532,101);
        else if (spr.Equals(sprs[1]))
            gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(259, 101);
        else if (spr.Equals(sprs[2]))
            gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(259, 101);
        else if (spr.Equals(sprs[3]))
            gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(362, 101);
        else if (spr.Equals(sprs[4]))
            gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(604, 101);
        else if (spr.Equals(sprs[5]))
            gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(604, 101);

        gameObject.GetComponent<Image>().sprite = spr;
        StartCoroutine(PerformanceCor(spr));
    }

    IEnumerator PerformanceCor(Sprite spr)
    {
        yield return new WaitForSeconds(2.2f);
        gameObject.GetComponent<Image>().sprite = null;
        gameObject.SetActive(false);
    }
}
