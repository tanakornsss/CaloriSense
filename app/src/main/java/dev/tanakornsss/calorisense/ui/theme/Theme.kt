package dev.tanakornsss.calorisense.ui.theme

import android.os.Build
import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.ExperimentalMaterial3ExpressiveApi
import androidx.compose.material3.MaterialExpressiveTheme
import androidx.compose.material3.Shapes
import androidx.compose.material3.darkColorScheme
import androidx.compose.material3.dynamicDarkColorScheme
import androidx.compose.material3.dynamicLightColorScheme
import androidx.compose.material3.expressiveLightColorScheme
import androidx.compose.material3.lightColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp

private val DarkColorScheme = darkColorScheme(
    primary = Purple80,
    secondary = PurpleGrey80,
    tertiary = Pink80
)

private val LightColorScheme = lightColorScheme(
    primary = Purple40,
    secondary = PurpleGrey40,
    tertiary = Pink40

    /* Other default colors to override
    background = Color(0xFFFFFBFE),
    surface = Color(0xFFFFFBFE),
    onPrimary = Color.White,
    onSecondary = Color.White,
    onTertiary = Color.White,
    onBackground = Color(0xFF1C1B1F),
    onSurface = Color(0xFF1C1B1F),
    */
)

@OptIn(ExperimentalMaterial3ExpressiveApi::class)
@Composable
fun CaloriSenseTheme(content:  @Composable () -> Unit) {
    val isDarkTheme = isSystemInDarkTheme()
    val supportsDynamicColors = Build.VERSION.SDK_INT >= Build.VERSION_CODES.S
    val colorScheme =
        when {
            supportsDynamicColors && isDarkTheme -> {
                dynamicDarkColorScheme(LocalContext.current)
            }
            supportsDynamicColors && !isDarkTheme -> {
                dynamicLightColorScheme(LocalContext.current)
            }
            isDarkTheme -> DarkColorScheme
            !isDarkTheme -> LightColorScheme
            else -> expressiveLightColorScheme()
        }

    val shapes = Shapes(largeIncreased = RoundedCornerShape(24.0.dp))
    MaterialExpressiveTheme(colorScheme = colorScheme, shapes = shapes) {
        content()
    }
}