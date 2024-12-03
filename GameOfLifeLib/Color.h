#pragma once

// Une séquence ANSI (ou code ANSI d'échappement)
// est une série de caractères spéciaux utilisée
// pour contrôler la mise en forme du texte dans les terminaux compatibles.
// Ces séquences sont standardisées et permettent d'ajouter des effets.

// Différence entre ASCII et ANSI :
// 
// ASCII => Est un standard pour l'encodage de caractères.
// Il ne contient pas de commandes pour formater ou styliser du texte dans un terminal.
// Les caractères d'échappement ASCII, comme \n(nouvelle ligne), \t(tabulation), et \b(retour en arrière),
// affectent principalement l'espacement ou le mouvement du curseur.
//
// ANSI => Est un standard pour contrôler les attributs visuels(couleurs, gras, souligné, etc.) 
// du texte dans un terminal via des séquences d'échappement comme \033[31m pour mettre du texte en rouge.

// ANSI (American National Standards Institute)
#define BOLD_TEXT "\033[1m"					// Gras
#define ITALIC_TEXT "\033[3m"				// Italique
#define UNDERLINE_TEXT "\033[4m"			// Souligné
#define STRIKETHROUGH_TEXT "\033[9m"		// Barré
#define INVERSE_TEXT "\033[7m"				// Inverse les couleurs
#define BLINK_TEXT "\033[5m"				// Clignotant
#define DIM_TEXT "\033[2m"					// Semi-gras
#define HIDDEN_TEXT "\033[8m"				// Masqué
#define DOUBLE_UNDERLINE_TEXT "\033[21m"	// Double soulignée

// Resets
#define RESET "\033[0m"
#define RESET_BOLD "\033[22m"
#define RESET_ITALIC "\033[23m"
#define RESET_UNDERLINE "\033[24m"
#define RESET_BLINK "\033[25m"
#define RESET_INVERSE "\033[27m"
#define RESET_STRIKETHROUGH "\033[29m"

// Text colors
#define BLACK_TEXT "\033[30m"
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define BLUE_TEXT "\033[34m"
#define MAGENTA_TEXT "\033[35m"
#define CYAN_TEXT "\033[36m"
#define WHITE_TEXT "\033[37m"

// Bold text colors
#define BLACK_BOLD_TEXT "\033[1;30m"
#define RED_BOLD_TEXT "\033[1;31m"
#define GREEN_BOLD_TEXT "\033[1;32m"
#define YELLOW_BOLD_TEXT "\033[1;33m"
#define BLUE_BOLD_TEXT "\033[1;34m"
#define MAGENTA_BOLD_TEXT "\033[1;35m"
#define CYAN_BOLD_TEXT "\033[1;36m"
#define WHITE_BOLD_TEXT "\033[1;37m"

// Background colors
#define BLACK_BG "\033[40m"
#define RED_BG "\033[41m"
#define GREEN_BG "\033[42m"
#define YELLOW_BG "\033[43m"
#define BLUE_BG "\033[44m"
#define MAGENTA_BG "\033[45m"
#define CYAN_BG "\033[46m"
#define WHITE_BG "\033[47m"

// High intensity text colors
#define BLACK_INTENSE_TEXT "\033[90m"
#define RED_INTENSE_TEXT "\033[91m"
#define GREEN_INTENSE_TEXT "\033[92m"
#define YELLOW_INTENSE_TEXT "\033[93m"
#define BLUE_INTENSE_TEXT "\033[94m"
#define MAGENTA_INTENSE_TEXT "\033[95m"
#define CYAN_INTENSE_TEXT "\033[96m"
#define WHITE_INTENSE_TEXT "\033[97m"

// High intensity background colors
#define BLACK_INTENSE_BG "\033[100m"
#define RED_INTENSE_BG "\033[101m"
#define GREEN_INTENSE_BG "\033[102m"
#define YELLOW_INTENSE_BG "\033[103m"
#define BLUE_INTENSE_BG "\033[104m"
#define MAGENTA_INTENSE_BG "\033[105m"
#define CYAN_INTENSE_BG "\033[106m"
#define WHITE_INTENSE_BG "\033[107m"

