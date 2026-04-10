"""Module for the submit component of the CICADA-3301 application."""

from commands.base_command import BaseCommand    

class SubmitCommand(BaseCommand):
    """Command implementation for the submit command."""
    name = "submit"
    description = "Submit a flag"
    usage = "submit <flag>"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        if len(args) < 1:
            print("Usage: submit <flag>")
            return

        flag = args[0]
        user = terminal.auth.get_current_user()
        
        # Salva o ID do desafio atual antes do submit
        current_challenge_before = terminal.ctf.get_current()
        challenge_id_before = current_challenge_before.get("id", 0)

        if terminal.ctf.submit_flag(flag, user):
            if challenge_id_before == 7:  # Último desafio principal foi completado
                print("🎉 PARABÉNS! Você completou o CTF de Segurança CICADA-3301! 🎉")
                print("Você demonstrou domínio em:")
                print("• Codificação e decodificação (Base64)")
                print("• Criptografia clássica (Cifra de César)")
                print("• Hashing e cracking de senhas (SHA256)")
                print("• Esteganografia (LSB Steganography)")
                print("• Análise forense de logs")
                print("• Escalação de privilégios")
                print("• Integração de técnicas de segurança")
                print("")
                print("Para encerrar o sistema, digite: shutdown")
            else:
                print("Flag correct")
                print("Next challenge unlocked")
        else:
            print("Flag incorrect!")
