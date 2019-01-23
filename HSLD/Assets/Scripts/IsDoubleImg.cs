using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IsDoubleImg : MonoBehaviour
{
    private Vector3 startingPos = new Vector3(640,200,0);
    private Vector3 endPos = new Vector3(1220,50,0);

    private float time_;

    IEnumerator MovingCor()
    {
        time_ = 0;
        while (true)
        {
            gameObject.transform.position = Vector3.Lerp(startingPos, endPos, time_);
            time_ += Time.deltaTime * 0.5f;
            yield return new WaitForEndOfFrame();
            if (time_ > 1f)
                break;
        }
        yield return new WaitForSeconds(0.5f);
        DiceSystem.instance_.gameObject.transform.parent.gameObject.SetActive(false);
    }

    private void OnEnable()
    {
        if (GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;

        gameObject.transform.position = startingPos;
        StartCoroutine(MovingCor());
    }
}
