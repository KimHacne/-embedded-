package com.example.embedded

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import androidx.core.view.isVisible
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase
import kotlinx.android.synthetic.main.activity_main.*
import org.jetbrains.anko.toast

class MainActivity : AppCompatActivity() {

    private val db = Firebase.database
    val answerRef = db.getReference("game")
    lateinit var input : String
    lateinit var answer : String
    lateinit var equal : String
    lateinit var start : String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        //파이어베이스 RDB가 변할 때마다 answer을 받아옴 answer 변수에 저장
        answerRef.addValueEventListener(object : ValueEventListener{
            override fun onDataChange(snapshot: DataSnapshot) {
                val map = snapshot.value as Map<*,*>
                answer=map["answer"].toString()
                start = map["start"].toString()
                Log.d("answer","$answer")
                Log.d("start","$start")

                //시작이 true가 되면 입력칸과 버튼이 보임
                if(start.equals("true")){
                    is_start.isVisible=false
                    num.isVisible=true
                    check.isVisible=true
                }
            }
            override fun onCancelled(error: DatabaseError) {
            }
        })


        //전송 버튼 클릭시 RDB에 input값 전달
        check.setOnClickListener {
            //num칸 공백검사
            if (num.text.isNullOrBlank()) {
                toast("빈칸은 안되요")
            } else {
                input = num.text.toString()
                Log.d("input", "$input")
                //answerRef.child("input").setValue(input)

                //answer변수값과 input변수값 비교 후 euqaul값 RDB전송
                if (answer == input) {
                    equal = "true"
                    answerRef.child("equal").setValue(equal)
                    Log.d("equal", "$equal")

                    //데이터 초기화
//                    answerRef.child("equal").setValue(false)
//                    answerRef.child("input").setValue("")
//                    answerRef.child("start").setValue(false)

                    //정답이면 SuccessActivity로 전환
                    val intent = Intent(this,SuccessActivity::class.java)
                    startActivity(intent)
                    finish()
                    overridePendingTransition(R.anim.slide_left_enter,R.anim.slide_left_exit)

                } else {
                    equal = "false"
                    answerRef.child("equal").setValue(equal)
                    Log.d("equal", "$equal")

                    //데이터 초기화
//                    answerRef.child("equal").setValue(false)
//                    answerRef.child("input").setValue("")
//                    answerRef.child("start").setValue(false)

                    //틀리면 FailActivity로 전환
                    val intent = Intent(this,FailActivity::class.java)
                    startActivity(intent)
                    finish()
                    overridePendingTransition(R.anim.slide_right_enter,R.anim.slide_right_exit)
                }
            }
        }

    }
}