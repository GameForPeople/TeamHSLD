using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DisplayText : MonoBehaviour
{
    private float time_;
    public string text;

    private void OnEnable()
    {
        gameObject.GetComponentInChildren<Text>().text = text;
        StartCoroutine(performance());
    }
    private void OnDisable()
    {
        gameObject.transform.GetChild(1).GetComponent<RectTransform>().sizeDelta = new Vector2(10, 100);
    }

    IEnumerator performance()
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime * 5f;
            gameObject.transform.GetChild(0).localPosition = Vector3.Lerp(new Vector3(-661.5f, 0,0), new Vector3(-5.4f, 0,0), time_);
            gameObject.transform.GetChild(0).GetComponent<RectTransform>().sizeDelta = Vector2.Lerp(new Vector2(25, 150), new Vector2(1340, 150), time_);
            yield return new WaitForEndOfFrame();
            if (time_ > 1.0f)
                break;
        }
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime * 5f;
            yield return new WaitForEndOfFrame();
            gameObject.transform.GetChild(1).GetComponent<RectTransform>().sizeDelta = Vector2.Lerp(new Vector2(10, 100),new Vector2(500,100), time_);            
            if (time_ > 1.0f)
                break;
        }
        yield return new WaitForSeconds(1.8f);
        gameObject.SetActive(false);
    }
}
