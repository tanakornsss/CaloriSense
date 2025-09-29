package dev.tanakornsss.calorisense

import androidx.activity.ComponentActivity
import androidx.compose.runtime.Composable
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import dev.tanakornsss.calorisense.ui.nav.CaloriSenseScreen
import dev.tanakornsss.calorisense.ui.screen.TokenGenScreen
import dev.tanakornsss.calorisense.ui.theme.CaloriSenseTheme

@Composable
fun CaloriSenseApp(context: ComponentActivity) {
    val navController = rememberNavController()
    
    CaloriSenseTheme {
        NavHost(
            navController = navController,
            startDestination = CaloriSenseScreen.TokenGen.name,
        ) {
            composable(CaloriSenseScreen.TokenGen.name) {
                TokenGenScreen(context)
            }
        }
    }
}