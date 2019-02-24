using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PointerBlink : MonoBehaviour
{
    private float time_;

    private void OnEnable()
    {
        StartCoroutine(Blink());
    }

    private void OnDisable()
    {
        StopCoroutine(Blink());
    }

    IEnumerator Blink()
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            gameObject.GetComponent<Image>().color = Color.Lerp(new Color(1, 1, 1, 1), new Color(1, 1, 1, 0), time_);

            if (time_ > 1)
                break;

            yield return new WaitForEndOfFrame();
        }

        time_ = 0;

        while (true)
        {
            time_ += Time.deltaTime;
            gameObject.GetComponent<Image>().color = Color.Lerp(new Color(1, 1, 1, 0), new Color(1, 1, 1, 1), time_);

            yield return new WaitForEndOfFrame();

            if (time_ > 1)
                break;
        }

        StartCoroutine(Blink());
    }
}
