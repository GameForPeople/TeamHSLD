using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardEffect : MonoBehaviour {
    public GameObject[] cardObject;
    public GameObject effectCard;
    public GameObject testMesh;
    Vector2 rotateAmount;
    float speed = 100;

    // Use this for initialization
    void Start () {
        //EventCardDeleteMesh(testMesh.transform, true);
        //EventCardInMesh(testMesh.transform, true);
        //EventDouble(testMesh.transform);
        //EventDefence(testMesh.transform, true);
    }

    public void EventDefence(Transform transform, bool bValue)
    {
        Vector3 pos = transform.position.normalized * 60;
        Quaternion firstQuaternion = Quaternion.Euler(0.0f, -90.0f, 70.0f);

        effectCard = Instantiate(cardObject[0], pos, firstQuaternion);

        StartCoroutine(Camera.main.GetComponent<CameraController>().movePositionEffect(transform.position));
        StartCoroutine(MoveUpDefence(transform, bValue));
    }

    public void EventDouble(Transform transform)
    {
        Vector3 pos = transform.position.normalized * 60;
        Quaternion firstQuaternion = Quaternion.Euler(0.0f, -90.0f, 70.0f);

        effectCard = Instantiate(cardObject[1], pos, firstQuaternion);
        StartCoroutine(Camera.main.GetComponent<CameraController>().movePositionEffect(transform.position));
        StartCoroutine(DestroyCard());
    }

    public void EventCardDeleteMesh(Transform transform, bool bValue)
    {
        Vector3 firstPosition;
        if (bValue) // 특수 카드 패????????????????
        {
            firstPosition = new Vector3(transform.position.x, transform.position.y - 10000, transform.position.z);
        }
        else // 상대 초상화에서 나와야 함
        {
            firstPosition = new Vector3(transform.position.x + 10000, transform.position.y + 10000, transform.position.z);
        }
        Quaternion firstQuaternion = Quaternion.Euler(0.0f, -90.0f, 70.0f);

        effectCard = Instantiate(cardObject[5], firstPosition, firstQuaternion);

        StartCoroutine(Camera.main.GetComponent<CameraController>().movePositionEffect(transform.position));
        StartCoroutine(MoveUpCard(transform, bValue));
    }

    public void EventCardInMesh(Transform transform, bool bValue)
    {
        Vector3 firstPosition;
        if (bValue) // 특수 카드 패????????????????
        {
            firstPosition = new Vector3(transform.position.x, transform.position.y - 10000, transform.position.z);
        }
        else // 상대 초상화에서 나와야 함
        {
            firstPosition = new Vector3(transform.position.x + 10000, transform.position.y + 10000, transform.position.z);
        }
        Quaternion firstQuaternion = Quaternion.Euler(0.0f, -90.0f, 70.0f);

        effectCard = Instantiate(cardObject[2], firstPosition, firstQuaternion);

        StartCoroutine(Camera.main.GetComponent<CameraController>().movePositionEffect(transform.position));
        StartCoroutine(MoveUpNoRotate(transform, bValue));
    }

    public IEnumerator DestroyCard()
    {
        yield return new WaitForSeconds(3.0f);

        Destroy(effectCard);
    }

    public IEnumerator MoveUpCard(Transform MeshTransform, bool bValue)
    {
        Vector3 pos = MeshTransform.position.normalized * 60;

        if (bValue == true)
        {
            while (effectCard.transform.position.y < pos.y - 3)
            {
                effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, pos, Time.deltaTime);
                yield return null;
            }
        }
        else
        {
            while (effectCard.transform.position.y > pos.y + 3)
            {
                effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, pos, Time.deltaTime);
                yield return null;
            }
        }

        StartCoroutine(MoveRotateCard(MeshTransform));
    }

    public IEnumerator MoveUpDefence(Transform MeshTransform, bool bValue)
    {
        Vector3 pos = MeshTransform.position.normalized * 60;

        if (bValue == true)
        {
            while (effectCard.transform.position.y < pos.y - 3)
            {
                effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, pos, Time.deltaTime);
                yield return null;
            }
        }
        else
        {
            while (effectCard.transform.position.y > pos.y + 3)
            {
                effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, pos, Time.deltaTime);
                yield return null;
            }
        }

        StartCoroutine(DestroyCard());
    }

    public IEnumerator MoveRotateCard(Transform MeshTransform)
    {
        Vector3 minScale = new Vector3(0.01f, 13, 13);
        float amount = 0;
        while (amount < 355.0f)
        {
            Vector3 rotate = Vector3.Cross(MeshTransform.position.normalized, MeshTransform.right).normalized;
            effectCard.transform.RotateAround(gameObject.transform.position, rotate * 120, speed * Time.deltaTime);

            effectCard.transform.localScale = Vector3.Lerp(effectCard.transform.localScale, minScale, Time.deltaTime);

            amount += speed * Time.deltaTime;
            yield return null;
        }
        StartCoroutine(MoveInMesh(effectCard, 1));
    }

    public IEnumerator MoveUpNoRotate(Transform MeshTransform, bool bValue)
    {
        Vector3 pos = MeshTransform.position.normalized * 60;

        if (bValue == true) {
            while (effectCard.transform.position.y < pos.y - 3)
            {
                effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, pos, Time.deltaTime);
                yield return null;
            }
        }
        else
        {
            while (effectCard.transform.position.y > pos.y + 3)
            {
                effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, pos, Time.deltaTime);
                yield return null;
            }
        }

        StartCoroutine(MoveInMesh(effectCard, 2));
    }

    public IEnumerator MoveInMesh(GameObject cardObj, int num)
    {
        Vector3 minScale = new Vector3(0.01f, 3, 3);
        while (effectCard.transform.localScale.y > minScale.y + 1)
        {
            cardObj.transform.localScale = Vector3.Lerp(effectCard.transform.localScale, minScale, Time.deltaTime);
            yield return null;
        }
        if (num == 1)
        {
            Camera.main.GetComponent<CameraShake>().ShakeOnce();
        }
        GameObject effectObj = gameObject.GetComponent<AllMeshController>().EffectObj[num];
        testMesh.GetComponent<MeshController>().EulerRotCalEffect(testMesh, effectObj, 1.01f);
        Destroy(effectCard);
    }
}