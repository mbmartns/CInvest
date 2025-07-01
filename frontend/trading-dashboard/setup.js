const { execSync } = require("child_process")
const fs = require("fs")
const path = require("path")

console.log("🚀 Configurando Trading Dashboard...\n")

// Função para executar comandos
function runCommand(command, description) {
  console.log(`📦 ${description}...`)
  try {
    execSync(command, { stdio: "inherit" })
    console.log(`✅ ${description} concluído!\n`)
  } catch (error) {
    console.error(`❌ Erro ao executar: ${description}`)
    console.error(error.message)
    process.exit(1)
  }
}

// Função para criar diretórios
function createDirectory(dirPath) {
  if (!fs.existsSync(dirPath)) {
    fs.mkdirSync(dirPath, { recursive: true })
    console.log(`📁 Diretório criado: ${dirPath}`)
  }
}

// Função para criar arquivo
function createFile(filePath, content) {
  const dir = path.dirname(filePath)
  createDirectory(dir)
  fs.writeFileSync(filePath, content)
  console.log(`📄 Arquivo criado: ${filePath}`)
}

// 1. Instalar dependências
runCommand("npm install", "Instalando dependências do Node.js")

// 2. Inicializar shadcn/ui
console.log("🎨 Configurando shadcn/ui...")

// Criar components.json
const componentsConfig = {
  $schema: "https://ui.shadcn.com/schema.json",
  style: "default",
  rsc: true,
  tsx: true,
  tailwind: {
    config: "tailwind.config.ts",
    css: "app/globals.css",
    baseColor: "slate",
    cssVariables: true,
    prefix: "",
  },
  aliases: {
    components: "@/components",
    utils: "@/lib/utils",
  },
}

createFile("components.json", JSON.stringify(componentsConfig, null, 2))

// 3. Criar estrutura de diretórios
createDirectory("app")
createDirectory("components/ui")
createDirectory("lib")

// 4. Criar arquivos de configuração do Tailwind
const tailwindConfig = `import type { Config } from "tailwindcss"

const config = {
  darkMode: ["class"],
  content: [
    './pages/**/*.{ts,tsx}',
    './components/**/*.{ts,tsx}',
    './app/**/*.{ts,tsx}',
    './src/**/*.{ts,tsx}',
  ],
  prefix: "",
  theme: {
    container: {
      center: true,
      padding: "2rem",
      screens: {
        "2xl": "1400px",
      },
    },
    extend: {
      colors: {
        border: "hsl(var(--border))",
        input: "hsl(var(--input))",
        ring: "hsl(var(--ring))",
        background: "hsl(var(--background))",
        foreground: "hsl(var(--foreground))",
        primary: {
          DEFAULT: "hsl(var(--primary))",
          foreground: "hsl(var(--primary-foreground))",
        },
        secondary: {
          DEFAULT: "hsl(var(--secondary))",
          foreground: "hsl(var(--secondary-foreground))",
        },
        destructive: {
          DEFAULT: "hsl(var(--destructive))",
          foreground: "hsl(var(--destructive-foreground))",
        },
        muted: {
          DEFAULT: "hsl(var(--muted))",
          foreground: "hsl(var(--muted-foreground))",
        },
        accent: {
          DEFAULT: "hsl(var(--accent))",
          foreground: "hsl(var(--accent-foreground))",
        },
        popover: {
          DEFAULT: "hsl(var(--popover))",
          foreground: "hsl(var(--popover-foreground))",
        },
        card: {
          DEFAULT: "hsl(var(--card))",
          foreground: "hsl(var(--card-foreground))",
        },
      },
      borderRadius: {
        lg: "var(--radius)",
        md: "calc(var(--radius) - 2px)",
        sm: "calc(var(--radius) - 4px)",
      },
      keyframes: {
        "accordion-down": {
          from: { height: "0" },
          to: { height: "var(--radix-accordion-content-height)" },
        },
        "accordion-up": {
          from: { height: "var(--radix-accordion-content-height)" },
          to: { height: "0" },
        },
      },
      animation: {
        "accordion-down": "accordion-down 0.2s ease-out",
        "accordion-up": "accordion-up 0.2s ease-out",
      },
    },
  },
  plugins: [require("tailwindcss-animate")],
} satisfies Config

export default config`

createFile("tailwind.config.ts", tailwindConfig)

// 5. Criar globals.css
const globalsCss = `@tailwind base;
@tailwind components;
@tailwind utilities;
 
@layer base {
  :root {
    --background: 0 0% 100%;
    --foreground: 222.2 84% 4.9%;

    --card: 0 0% 100%;
    --card-foreground: 222.2 84% 4.9%;
 
    --popover: 0 0% 100%;
    --popover-foreground: 222.2 84% 4.9%;
 
    --primary: 222.2 47.4% 11.2%;
    --primary-foreground: 210 40% 98%;
 
    --secondary: 210 40% 96%;
    --secondary-foreground: 222.2 84% 4.9%;
 
    --muted: 210 40% 96%;
    --muted-foreground: 215.4 16.3% 46.9%;
 
    --accent: 210 40% 96%;
    --accent-foreground: 222.2 84% 4.9%;
 
    --destructive: 0 84.2% 60.2%;
    --destructive-foreground: 210 40% 98%;

    --border: 214.3 31.8% 91.4%;
    --input: 214.3 31.8% 91.4%;
    --ring: 222.2 84% 4.9%;
 
    --radius: 0.5rem;
  }
 
  .dark {
    --background: 222.2 84% 4.9%;
    --foreground: 210 40% 98%;
 
    --card: 222.2 84% 4.9%;
    --card-foreground: 210 40% 98%;
 
    --popover: 222.2 84% 4.9%;
    --popover-foreground: 210 40% 98%;
 
    --primary: 210 40% 98%;
    --primary-foreground: 222.2 47.4% 11.2%;
 
    --secondary: 217.2 32.6% 17.5%;
    --secondary-foreground: 210 40% 98%;
 
    --muted: 217.2 32.6% 17.5%;
    --muted-foreground: 215 20.2% 65.1%;
 
    --accent: 217.2 32.6% 17.5%;
    --accent-foreground: 210 40% 98%;
 
    --destructive: 0 62.8% 30.6%;
    --destructive-foreground: 210 40% 98%;
 
    --border: 217.2 32.6% 17.5%;
    --input: 217.2 32.6% 17.5%;
    --ring: 212.7 26.8% 83.9%;
  }
}
 
@layer base {
  * {
    @apply border-border;
  }
  body {
    @apply bg-background text-foreground;
  }
}`

createFile("app/globals.css", globalsCss)

// 6. Criar utils
const utilsContent = `import { type ClassValue, clsx } from "clsx"
import { twMerge } from "tailwind-merge"
 
export function cn(...inputs: ClassValue[]) {
  return twMerge(clsx(inputs))
}`

createFile("lib/utils.ts", utilsContent)

// 7. Criar layout.tsx
const layoutContent = `import type { Metadata } from "next"
import { Inter } from 'next/font/google'
import "./globals.css"

const inter = Inter({ subsets: ["latin"] })

export const metadata: Metadata = {
  title: "Trading Dashboard",
  description: "Dashboard para monitoramento de robô de trading automatizado",
}

export default function RootLayout({
  children,
}: {
  children: React.ReactNode
}) {
  return (
    <html lang="pt-BR">
      <body className={inter.className}>{children}</body>
    </html>
  )
}`

createFile("app/layout.tsx", layoutContent)

// 8. Criar tsconfig.json
const tsconfigContent = `{
  "compilerOptions": {
    "lib": ["dom", "dom.iterable", "es6"],
    "allowJs": true,
    "skipLibCheck": true,
    "strict": true,
    "noEmit": true,
    "esModuleInterop": true,
    "module": "esnext",
    "moduleResolution": "bundler",
    "resolveJsonModule": true,
    "isolatedModules": true,
    "jsx": "preserve",
    "incremental": true,
    "plugins": [
      {
        "name": "next"
      }
    ],
    "baseUrl": ".",
    "paths": {
      "@/*": ["./*"]
    }
  },
  "include": ["next-env.d.ts", "**/*.ts", "**/*.tsx", ".next/types/**/*.ts"],
  "exclude": ["node_modules"]
}`

createFile("tsconfig.json", tsconfigContent)

// 9. Criar next.config.js
const nextConfigContent = `/** @type {import('next').NextConfig} */
const nextConfig = {}

module.exports = nextConfig`

createFile("next.config.js", nextConfigContent)

// 10. Instalar componentes shadcn/ui
const components = ["card", "badge", "tabs", "scroll-area", "button", "input", "label", "separator"]

components.forEach((component) => {
  runCommand(`npx shadcn@latest add ${component} --yes`, `Instalando componente ${component}`)
})

console.log("🎉 Setup concluído com sucesso!")
console.log("\n📋 Próximos passos:")
console.log("1. Copie o código do dashboard para app/page.tsx")
console.log("2. Execute: npm run dev")
console.log("3. Acesse: http://localhost:3000")
console.log("\n🚀 Seu Trading Dashboard está pronto para usar!")
