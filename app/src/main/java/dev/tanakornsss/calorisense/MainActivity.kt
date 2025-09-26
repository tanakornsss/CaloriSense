package dev.tanakornsss.calorisense

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge

class MainActivity : ComponentActivity() {
    companion object {
        init {
            System.loadLibrary("omp")
            System.loadLibrary("calorisense")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            CaloriSenseApp(this)
        }
    }
}