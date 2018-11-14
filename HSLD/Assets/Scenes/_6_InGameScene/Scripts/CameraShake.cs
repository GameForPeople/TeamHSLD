using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraShake : MonoBehaviour {

    public IEnumerator Shake(float duration, float magnitude)
    {
        Vector3 originPos = transform.localPosition;

        float elapsed = 0.0f;

        while (elapsed < duration)
        {
            float x = Random.Range(-1f, 1f) * magnitude;
            float y = Random.Range(-1f, 1f) * magnitude;
            float z = Random.Range(-1f, 1f) * magnitude;

            transform.localPosition = new Vector3(originPos.x + x, originPos.y + y, originPos.z + z);
            Debug.Log(transform.localPosition);

            elapsed += Time.deltaTime;

            yield return null;
        }

        transform.localPosition = originPos;
    }
}
