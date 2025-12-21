import { defineConfig } from 'vitepress'

export default defineConfig({
    title: 'libcpprime',
    description: 'An efficient C++ implementation of a primality test optimized for 64-bit integers.',
    cleanUrls: false,
    themeConfig: {
        nav: [
            { text: 'Home', link: '/' },
            { text: 'Usage', link: '/usage/is-prime' },
            { text: 'Guide', link: '/guide/requirements', },
        ],
        sidebar: {
            '/': [
                {
                    text: 'Usage',
                    items: [
                        { text: 'IsPrime', link: '/usage/is-prime' },
                        { text: 'IsPrimeNoTable', link: '/usage/is-prime-no-table' },
                        { text: 'Feature Test Macros', link: '/usage/feature-test-macros' }
                    ]
                },
                {
                    text: 'Guide',
                    items: [
                        { text: 'Requirements', link: '/guide/requirements' },
                        { text: 'Compilation', link: '/guide/compilation' },
                        { text: 'Benchmarks', link: '/guide/benchmarks' }
                    ]
                },
                {
                    text: 'Other',
                    items: [
                        { text: 'Releases', link: '/releases' },
                        { text: 'References', link: '/references' }
                    ]
                }
            ]
        },
        footer: {
            message: 'Released under the MIT License except where noted.',
            copyright: 'Copyright © 2024-present Rac75116'
        },
        socialLinks: [{ icon: 'github', link: 'https://github.com/Rac75116/libcpprime' }]
    }
})
