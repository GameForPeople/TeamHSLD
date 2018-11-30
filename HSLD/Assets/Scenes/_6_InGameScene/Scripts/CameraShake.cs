using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraShake : MonoBehaviour
{
    public static bool isCameraShake;
    public GameObject canvas;
    public List<RectTransform> UIList;
    Vector2[] originList;

    void Start()
    {
        canvas = GameObject.Find("Canvas");
        isCameraShake = false;
        canvas.GetComponentsInChildren<RectTransform>(UIList);
        //Debug.Log(canvas.GetComponentsInChildren<RectTransform>());
        //UIList.RemoveAt(0);

        originList = new Vector2[UIList.Count];
    }

    public void ShakeOnce()
    {
        StartCoroutine(Shake(0.5f, 0.5f));
    }

    public IEnumerator Shake(float duration, float magnitude)
    {
        isCameraShake = true;
        Vector3 originPos = transform.localPosition;
        for (int i = 0; i < UIList.Count; i++)
        {
            originList[i] = UIList[i].transform.localPosition;
        }

        float elapsed = 0.0f;

        while (elapsed < duration)
        {
            float x = Random.Range(-1f, 1f) * magnitude;
            float y = Random.Range(-1f, 1f) * magnitude;
            float z = Random.Range(-1f, 1f) * magnitude;

            transform.localPosition = new Vector3(originPos.x + x, originPos.y + y, originPos.z + z);
            //Debug.Log(transform.localPosition);

            elapsed += Time.deltaTime;
            
            for(int i = 0; i < UIList.Count; i++)
            {
                UIList[i].transform.localPosition = new Vector2(originList[i].x + x*2, originList[i].y + y*2);
            }

            yield return null;
        }

        transform.localPosition = originPos;
        for (int i = 0; i < UIList.Count; i++)
        {
            UIList[i].transform.localPosition = originList[i];
        }
        isCameraShake = false;
    }

    public IEnumerator Shake(float duration, float magnitude, int linkednum)
    {
        isCameraShake = true;
        Vector3 originPos = transform.localPosition;
        for (int i = 0; i < UIList.Count; i++)
        {
            originList[i] = UIList[i].transform.localPosition;
        }

        float elapsed = 0.0f;

        while (elapsed < duration)
        {
            float x = Random.Range(-1f, 1f) * magnitude;
            float y = Random.Range(-1f, 1f) * magnitude;
            float z = Random.Range(-1f, 1f) * magnitude;

            transform.localPosition = new Vector3(originPos.x + x, originPos.y + y, originPos.z + z);
            //Debug.Log(transform.localPosition);

            elapsed += Time.deltaTime;

            for (int i = 0; i < UIList.Count; i++)
            {
                UIList[i].transform.localPosition = new Vector2(originList[i].x + x * 2, originList[i].y + y * 2);
            }

            yield return null;
        }

        transform.localPosition = originPos;
        for (int i = 0; i < UIList.Count; i++)
        {
            UIList[i].transform.localPosition = originList[i];
        }
        isCameraShake = false;

        AllMeshController.myPlanet.GetComponent<AllMeshController>().InitLinkedMesh(linkednum);
    }
}