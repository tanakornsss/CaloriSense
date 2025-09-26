package dev.tanakornsss.calorisense

import androidx.activity.ComponentActivity
import androidx.compose.runtime.Composable
import dev.tanakornsss.calorisense.ui.screen.TokenGenScreen
import dev.tanakornsss.calorisense.ui.theme.CaloriSenseTheme

@Composable
fun CaloriSenseApp(context: ComponentActivity) {
    CaloriSenseTheme {
        TokenGenScreen(context)
    }
}