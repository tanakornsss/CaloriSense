package dev.tanakornsss.calorisense

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import dev.tanakornsss.calorisense.ui.theme.CaloriSenseTheme

class MainActivity : ComponentActivity() {
    companion object {
        init {
            System.loadLibrary("calorisense")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            CaloriSenseTheme {
                GemmaTestScreen()
            }
        }
    }
}