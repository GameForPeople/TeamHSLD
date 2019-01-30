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
        gameObject.GetComponent<Text>().text = text;
        StartCoroutine(performance());
    }

    IEnumerator performance()
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime * 5f;
            yield return new WaitForEndOfFrame();
            gameObject.GetComponent<RectTransform>().sizeDelta = Vector2.Lerp(new Vector2(10, 10),new Vector2(300,200), time_);            
            if (time_ > 1.0f)
                break;
        }
        yield return new WaitForSeconds(2f);
        time_ = 0;
        //while (true)
        //{
        //    time_ += Time.deltaTime * 0.5f;
        //    yield return new WaitForEndOfFrame();
        //    gameObject.GetComponent<RectTransform>().sizeDelta = Vector2.Lerp(new Vector2(300, 200), new Vector2(10, 10), time_);
        //    if (time_ > 1.0f)
        //        break;
        //}
        gameObject.SetActive(false);
    }
}
